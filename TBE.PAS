{$A+,B+,D-,E-,F-,G-,I-,L-,N-,O-,R-,S-,V-,X+}
{$M 16384,0,320000}
program canontbe;

uses dos,crt;

const ctstimeout=300;
      rectimeout=800;

var i:longint;
    buffer : array[0..8191] of byte;
    cmdbuf : array[0..10] of byte;
    b,h,b1,b2:byte;
    zstart,zend,zlen,code:word;
    s1,s2,filename:string;
    c1:char;
    f1:file of byte;
    f:text;
    trxd,ier,lcr,mcr,lsr,msr:word;
    ComPorts : array[1..4] of word;


procedure printusage;
begin
  writeln;
  writeln('tbe V 0.22   98/1/02  (C)  Michael Rothe');
  writeln;
  writeln('communication program for Canon Technical Back E with Interface Unit TB');
  writeln('supposed to be run under plain DOS or Win95 DOS box (may work...)');
  writeln;
  writeln('usage   :  tbe port command [filename]');
  writeln;
  writeln('port    :  1..4 (for COM1..COM4)');
  writeln('command :  d - dump full 8 KByte TB-E memory (raw data)');
  writeln('           f - set film cartridge counter (give value instead of filename)');
  writeln('           i - ifu-style exposure recording area dump (ifu file format)');
  writeln('           n - list notes and imprint data   (text)');
{  writeln('           s - list various TB-E settings    (text)');}
  writeln;
  writeln('results will be written in dump_tbe.dat if filename is omitted');
  writeln;
end;

procedure initPortAdresses(port:byte);
begin
  trxd := ComPorts[port];
  ier  := trxd+1;
  lcr  := trxd+3;
  mcr  := trxd+4;
  lsr  := trxd+5;
  msr  := trxd+6;
end;

procedure init24;
begin
  port[lcr] := $80; {bitrate setzen}
  port[trxd]:= $30; {bitrate 2400}
  port[ier] := $00;
  port[lcr] := $07; {8n2}
  port[ier] := $00; {keine interrupts}
end;

function gettickms: word;
var h,m,s,c: word;
begin
  GetTime(h,m,s,c);
  gettickms:=s*1000+c*10;
end;

function getdiffms(since: word) : word;
var now: word;
begin
  now:=gettickms;
  if now>=since then getdiffms:=now-since else getdiffms:=60000-(since-now);
end;

procedure mydelay(time: word);
var start,diff: word;
begin
  start:=gettickms;
  repeat diff:=getdiffms(start); until diff>=time;
end;

procedure sendbyte(b:byte);
begin
  repeat until (port[lsr] and $20) <> 0 ;  {thre pollen}
  port[trxd]:=b;
end;

function getbyte(var b:byte) : byte;
var h:byte;
    start,time:word;
begin
  time := 0;
  start:=gettickms;
  repeat
    h:=port[lsr];
    time := getdiffms(start);
  until ((h and $01) <> 0) or (time >= rectimeout) ;  {rdr pollen}
  if (time >= rectimeout) then
  begin
    b:=0;
    getbyte:=255;
    exit;
  end;
  b:=port[trxd];
  getbyte := h;
end;

procedure clearrdr;
var h:byte;
    i:word;
begin
  i:=0;
  while (port[lsr] and $01) <> 0 do
  begin
    h:=port[trxd];
    inc(i);
  end;
{  writeln('clearrdr:',i);}
end;

procedure setrts;
begin
  port[mcr]:=$02;
end;

procedure clearrts;
begin
  port[mcr]:=$00;
end;

function wait4ctson: boolean;
var h:byte;
    start,time:word;
begin
  start:=gettickms;
  repeat
    h:=port[msr];
    time:=getdiffms(start);
  until ((h and $10) <> 0) or (time >= ctstimeout) ; { cts pollen}
{  writeln('wait4ctson time:',time);}
  wait4ctson:= time < ctstimeout;
end;

function wait4ctsoff: boolean;
var h:byte;
    start,time:word;
begin
  start:=gettickms;
  repeat
    h:=port[msr];
    time:=getdiffms(start);
  until ((h and $10) = 0) or (time >= ctstimeout) ; { cts pollen}
{  writeln('wait4ctsoff time:', time);}
  wait4ctsoff:= time < ctstimeout;
end;

procedure sendbuf(s,l:word);
var i:word;
begin
  i:=0;
  while i<l do begin sendbyte(buffer[s+i]); inc(i) end;
end;

procedure sendcmd;
var i:word;
begin
  for i:=0 to 5 do sendbyte(cmdbuf[i]);
end;

procedure getbuf(s,l:word);
var b,h:byte;
    s1:string;
    i: word;
begin
  i:=0;
  while i<=l do
  begin
    h:=getbyte(b);
    if h=255 then begin
      writeln('get byte timeout at byte ', i, ' of ',l);
      exit;
    end;
    buffer[s+i]:=b;
    inc(i);
  end;
end;

function execcmd(var rval: byte) : boolean;
var i,s,l: word;
begin
  setrts;   if NOT wait4ctson  then begin rval:=1; execcmd:=False; exit end;
  clearrts; if NOT wait4ctsoff then begin rval:=2; execcmd:=False; exit end;

  sendcmd;

  if NOT wait4ctson  then begin rval:=3; execcmd:=False; exit end;
  if NOT wait4ctsoff then begin rval:=4; execcmd:=False; exit end;

  if cmdbuf[10]=0 then begin rval:=0; execcmd:=True; exit end;

  s:=cmdbuf[6]+256*cmdbuf[7];
  l:=cmdbuf[8]+256*cmdbuf[9];

  mydelay(300);

  setrts;
  if NOT wait4ctson  then begin rval:=5; execcmd:=False; exit end;
  clearrts;

  port[lsr]:=$60; { ??? }

  if cmdbuf[10]=2 then
  begin
    if NOT wait4ctsoff then begin rval:=10; execcmd:=False; exit end;
    sendbuf(s,l);
    if NOT wait4ctson  then begin rval:=6; execcmd:=False; exit end;
    if NOT wait4ctsoff then begin rval:=7; execcmd:=False; exit end;
    rval:=2; execcmd:=True;
    exit;
  end;

  if cmdbuf[10]=1 then
  begin
    getbuf(s,l);
    if NOT wait4ctson  then begin rval:=8; execcmd:=False; exit end;
    if NOT wait4ctsoff then begin rval:=9; execcmd:=False; exit end;
    rval:=1; execcmd:=True;
    exit;
  end;

  rval:=3; execcmd:=True;
end;

function bcdbyte(i:byte):byte;
begin
  i:=i mod 100;
  bcdbyte:=(i mod 10)+16*(i div 10);
end;

function hex(b:byte):string;
const hexstring : string = '0123456789ABCDEF';
begin
  hex:=hexstring[(b div 16)+1]+hexstring[(b mod 16)+1];
end;

procedure showhex(b:byte);
var a,z,s:word;
    h:byte;
begin
  a:=0;
  for z:=0 to 15 do begin
    write(hex(b),hex(a),'  ');
    for s:=0 to 15 do begin
      write(hex(buffer[a+s]),' ');
    end;
    write ('| ');
    for s:=0 to 15 do begin
      h:=buffer[a+s];
      if h<32 then h:=Ord('-');
      write(Chr(h));
    end;
    a:=a+16;
    writeln;
  end;
end;

function getnotetext(num:word) : string;
var adr,i:word;
    s:string;
begin
  s:='';
  adr:=num*32;
  for i:=0 to 29 do
  begin
    if buffer[adr+i] > 31
      then s:=s+chr(buffer[adr+i])
      else s:=s+'*';
  end;
  getnotetext:=s;
end;

begin
  ComPorts[1] := memw[$40:$0];
  ComPorts[2] := memw[$40:$2];
  ComPorts[3] := memw[$40:$4];
  ComPorts[4] := memw[$40:$6];
  b:=0; c1:=' ';
  cmdbuf[0]:=$5A;
  for i:=1 to 10 do cmdbuf[i]:=0;
  for i:=0 to 8191 do buffer[i]:=0;
  if ParamCount < 2 then begin printusage; exit end;
  Val(ParamStr(1),b,code);
  if (b < 1) or (b > 4) then begin printusage; exit end;
  if ComPorts[b] = 0 then
  begin
    writeln('COM',b,' nicht gefunden, bitte verwenden sie einen anderen COM-Port');
    exit;
  end;
  initPortAdresses(b);
  init24;
  clearrdr;

  s1:=ParamStr(2);
  c1:=UpCase(s1[1]);

  filename:='';
  if Paramcount >= 3 then filename:=ParamStr(3);
  if filename='' then
  begin
    writeln('using default filename dump_tbe.dat');
    filename:='dump_tbe.dat';
  end;

  case c1 of
    'I' : begin
	    cmdbuf[1]:=$A3;
	    cmdbuf[2]:=$3A;
	    cmdbuf[3]:=$00;
	    cmdbuf[4]:=$3F;
	    cmdbuf[5]:=$00;
	    cmdbuf[8]:=$06;
	    cmdbuf[10]:=1;
	    if NOT execcmd(b1) then
	    begin
	      writeln('I/O error, code:', b1);
	      exit;
	    end;
	    zstart:=buffer[1]+256*buffer[0];
	    zend:=buffer[5]+256*buffer[4]-1;
	    zlen:=(1+zend)-zstart;
	    if zlen = 0 then begin
	      writeln('no exposure data in storage, looking for old data...');
	      zend:=buffer[3]+256*buffer[2]-1;
	      zlen:=(1+zend)-zstart;
	      if zlen = 0 then begin
		writeln('no old exposure data found');
		exit;
	      end
	      else writeln('old exposure data found');
	    end;
	    cmdbuf[1]:=$A3;
	    cmdbuf[2]:=buffer[1];
	    cmdbuf[3]:=buffer[0]-$80;
	    cmdbuf[4]:=zend mod 256;
	    cmdbuf[5]:=(zend div 256)-$80;
	    cmdbuf[8]:=zlen mod 256;
	    cmdbuf[9]:=zlen div 256;
	    cmdbuf[10]:=1;
	    if NOT execcmd(b1) then
	    begin
	      writeln('I/O error, code:', b1);
	      exit;
	    end;
	    write('data transfer successful, writing to file...');
	    assign(f1, filename);
	    rewrite(f1);
	    write(f1,cmdbuf[8]); write(f1,cmdbuf[9]);
	    b2:=0; for i:=0 to 191 do write(f1,b2);
	    b2:=$04; write(f1,b2);
	    for i:=0 to zlen-1 do write(f1,buffer[i]);
	    b2:=$FF; for i:=0 to 3 do write(f1, b2);
	    close(f1);
	    writeln(zlen+199, ' bytes written.');
	  end;

    'F' : begin
	    if ParamCount < 3 then begin printusage; exit end;
	    Val(ParamStr(3),i,code);
	    if (i < 1) or (i > 999) then begin printusage; exit end;
	    b2:=bcdbyte(i div 100);
	    cmdbuf[1]:=$A7;
	    cmdbuf[2]:=$36;
	    cmdbuf[3]:=$80;
	    cmdbuf[4]:=b2;
	    cmdbuf[5]:=$FF;
	    cmdbuf[10]:=0;
	    if NOT execcmd(b1) then
	    begin
	      writeln('I/O error, code:', b1);
	      exit;
	    end;
	    b2:=bcdbyte(i mod 100);
	    cmdbuf[1]:=$A7;
	    cmdbuf[2]:=$37;
	    cmdbuf[3]:=$80;
	    cmdbuf[4]:=b2;
	    cmdbuf[5]:=$FF;
	    cmdbuf[10]:=0;
	    if NOT execcmd(b1) then
	    begin
	      writeln('I/O error, code:', b1);
	      exit;
	    end;
	    writeln('data transfer successful');
	  end;

    'S' : begin
	    writeln('sorry, settings list not implemented yet...');
	    exit;

	    cmdbuf[1]:=$A3;
	    cmdbuf[4]:=$FF;
	    cmdbuf[9]:=$01;
	    cmdbuf[10]:=1;
	    if NOT execcmd(b1) then
	    begin
	      writeln('I/O error, code:', b1);
	      exit;
	    end;
	    write('data transfer successful, writing to file...');
	    assign(f, filename);
	    rewrite(f);

	    close(f);
	    writeln(' ?? lines written.');
	  end;

    'D' : begin
	    cmdbuf[1]:=$A3;
	    cmdbuf[4]:=$FF;
	    cmdbuf[5]:=$1F;
	    cmdbuf[9]:=$20;
	    cmdbuf[10]:=1;
	    if NOT execcmd(b1) then
	    begin
	      writeln('I/O error, code:', b1);
	      exit;
	    end;
	    write('data transfer successful, writing to file...');
	    assign(f1, filename);
	    rewrite(f1);
	    for i:=0 to 8191 do write(f1,buffer[i]);
	    close(f1);
	    writeln(' 8192 bytes written.');
	  end;

    'N' : begin
	    cmdbuf[1]:=$A3;
	    cmdbuf[3]:=$01;
	    cmdbuf[4]:=$FF;
	    cmdbuf[5]:=$05;
	    cmdbuf[9]:=$05;
	    cmdbuf[10]:=1;
	    if NOT execcmd(b1) then
	    begin
	      writeln('I/O error, code:', b1);
	      exit;
	    end;
	    write('data transfer successful, writing to file...');
	    assign(f, filename);
	    rewrite(f);
	    for b2:=0 to 3 do
	    begin
	      s1:=getnotetext(b2);
	      writeln(f,'Note   ',b2+1,' [',s1,']');
	    end;
	    for b2:=1 to 36 do
	    begin
	      s1:=getnotetext(b2+3);
	      writeln(f,'Frame ',b2:2,' [',s1,']');
	    end;
	    close(f);
	    writeln(' 40 lines written.');
	  end;

    'X' : begin
	    c1:=' ';
	    repeat
	      for i:=1 to 10 do cmdbuf[i]:=0;
	      if c1='R' then begin clrscr; showhex(b) end;
	      clearrdr;
	      write('Command (Read,Write,SW-1,Busy,Clear,eXit) : '); readln(c1);
	      c1:=UpCase(c1);
	      case c1 of
		'R' : begin
			writeln('Read and display 256 byte from TB-E memory');
			write('Address (Hi Byte, use $xx for hex input) :'); readln(b);
			cmdbuf[1]:=$A3;
			cmdbuf[2]:=0;
			cmdbuf[3]:=b;
			cmdbuf[4]:=$FF;
			cmdbuf[5]:=b;
			cmdbuf[8]:=0; cmdbuf[9]:=1;
			cmdbuf[10]:=1;
			if execcmd(b1)
			  then writeln('Success, code:', b1)
			  else writeln('Failed, code:', b1);
		      end;

		'W' : begin
			writeln('Write a single byte into TB-E memory');
			write('Address (use $xxxx for hex input) :'); readln(zstart);
			write('Value   (use $xx   for hex input) :'); readln(b);
			cmdbuf[1]:=$A7;
			cmdbuf[2]:=zstart mod 256;
			cmdbuf[3]:=zstart div 256;
			cmdbuf[4]:=b;
			cmdbuf[5]:=$FF;
			if execcmd(b1)
			  then writeln('Success, code:', b1)
			  else writeln('Failed, code:', b1);
		      end;

		'S' : begin
			cmdbuf[1]:=$A5;
			cmdbuf[2]:=1;
			if execcmd(b1)
			  then writeln('Success, code:', b1)
			  else writeln('Failed, code:', b1);
		      end;

		'B' : begin
			cmdbuf[1]:=$AD;
			cmdbuf[2]:=1;
			if execcmd(b1)
			  then writeln('Success, code:', b1)
			  else writeln('Failed, code:', b1);
		      end;

		'C' : begin
			cmdbuf[1]:=$AD;
			cmdbuf[2]:=$0C;
			if execcmd(b1)
			  then writeln('Success, code:', b1)
			  else writeln('Failed, code:', b1);
		      end;

		'X' : halt;
	      end;
	    until false;
	  end;
    else  printusage;
  end;
end.