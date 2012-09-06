#ifndef xbkstring
#define xbkstring

#include<string>
#include<stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#include <map>
#include <iostream>

#include <xbk/xbkdebug.c>
#include <xbk/xbkqs.c>

extern int errno;
#include "../include/xbkstringutil.h"


/*
todo:
die Basisklasse fuer die CGI Behandlung kann die Multimap
bekommen und die dafür vorgesehenen Routinen. Sie sind 
für sich allein gestellt und können von daher übernommen werden in
die base Klasse.
*/


/*

#define DEBUGME
#define QS

#include "xdebug.c"
#include "xqs.c"
*/

/*
----------------------------------------------
*/

/*
Getenv fuehrt bei Leerwerten zum Absturz,
deswegen meine eigene Implementation
*/
string xbkstringutil::xbkgetenv(string envi)
{
char *z;
z=getenv(envi.c_str());
if(z){return z;}else{return "";}
}



/*
--------------------
stripslashes
Entfernt \ bei " \ oder '
--------------------
*/

string xbkstringutil::stripslashes(string text)
{

if(text==""){return "";};
qs_start();

register const char *cstring;
register char *ziel;
register int z=0;

cstring=text.c_str();
ziel = new char[strlen(cstring)+1];

while(*cstring)
{
if( (ziel[z]=*cstring++)==92)
   {
    if( *cstring==34 || *cstring==39 || *cstring==92)
    z--;
   };
z++;
}

ziel[z]=0;
qs_stop();
return ziel;

}


/*
--------------------
addslashes
Fuegt \ bei " \ oder ' an
--------------------
*/

string  xbkstringutil::addslashes(string text)
{
qs_start();
register int x;
register int t=0;
register char *tmp = new char[text.length()*2];
register const char *txt=text.c_str();
//printf("\nlashcall mit %s\n ",txt);
register int l = text.length();

while(*txt)
{
//printf("bearbeite Zeichen %c\n",*txt);
 if(*txt == 34 || *txt == 39 || *txt == 92) {tmp[t++]=92;}

 tmp[t++]=*txt;
 *txt++;
}
tmp[t]=0;
//printf("lavsjkdf --- %s\n",tmp);
qs_stop();
return tmp;
}

/*
entfernt Leerzeichen und Returns am Anfang und am Ende

Beispiel-Code:

string irgendwas;
irgendwas="  ";
irgendwas=trim(irgendwas);
cout << ":::" << irgendwas << ":::";


*/ 

string  xbkstringutil::trim(string text)
{
qs_start();
register const char *txt = text.c_str();
register char *tmp;
register int t=0;
int l = strlen(txt);

//Zeichen am Anfang uebergehen
tmp=new char[l];
while(*txt) { if ( (*txt!=' ' && *txt!=13 && *txt!=10) || *txt==0){break;} *txt++; }

//wir gehen zum Ende
register const char *ende=txt+l-1;

//rueckwaerts suchen bis ein passendes Zeichen gefunden ist

while(*ende)  
 { 
 if( *ende != ' ' && *ende != 13 && *ende != 10){*ende++;break;}
 *ende--;
 }

while(txt!=ende) { tmp[t++]=*txt++; } 
tmp[t+1]=0;
qs_stop();
return tmp;
}




string  xbkstringutil::str_replace(string searchfor,string replacetxt,string origtxt )
{
qs_start();
DEBUG("STR_REPLACE START mit String:");
register const char * sf=searchfor.c_str();
register const char * rt=replacetxt.c_str();
register const char * ot=origtxt.c_str();

int l = strlen(ot);
int memcounter = l+(3*strlen(rt));//Speicher = Originaltext+3fachen Replacetxt
//int otcounter  = l;
register int restspeicher = memcounter;
register int restspeicher_buffer;

register int t=0;
register int t_buffer;

register const char *ot_buffer;
register const char *rt_buffer;
register const char *sf_buffer;

register int rt_len=strlen(rt);
register int rt_counter;


register int sf_len=strlen(sf);
register int sf_counter;

//register char * tmp = new char(memcounter);
register char * tmp = (char*)malloc(memcounter);

register int pagemul=10;//wird +1 erhoeht nach jedem realloc!

while(*ot)
{
DEBUG("WHILE *OT Schleife Start an TMP-POS %i\n",t);
DEBUG("WHILE Start TMP: %s\n",tmp);
DEBUG("WHILE Restspeicherbytes: %i\n",restspeicher);
/*
Stimmt Anfang und Ende mit Suchbegriff ueberein? Performance rulez!
*/
if(*ot==*sf && *ot+sf_len==*sf+sf_len)
{
DEBUG("Erster und letzter Punkt im Suchbegriff passen schonmal");
DEBUG("vergleiche Zeichen %c mit Zeichen %c\n",*ot,*sf );

   //jetzt muss der gesamte Bereich verglichen werden
   //wenn er passt, dann muessen wir substituieren

    ot_buffer=ot;		//Quelltext "hallo #irgendwas#"
    rt_buffer=rt;		//"ERSETZUNGSWERT"
    rt_counter=0;
    sf_counter=0;
    sf_buffer=sf;		//"#irgendwas#"  
    t_buffer=t;
    restspeicher_buffer=restspeicher;

    

    while(*sf_buffer)
     {


DEBUG("MEMORY CHECK, aktueller String: __%s__\n",tmp);
DEBUG("Restspeicher: %i Minimum: %i \n",restspeicher,rt_len);
 //Restspeicher pruefen
   if (restspeicher<(rt_len))
     {
       DEBUG("+++ WARNUNG, REALLOCATION\n");
       DEBUG("Aktueller MEMCOUNTER Bytes: %i\n",memcounter);
       memcounter=memcounter+(rt_len*pagemul);
       DEBUG("Reallokation auf %i Bytes\n",memcounter);
       DEBUG("vor realloc: __%s__\n",tmp);
       //char *stmp = strdup(tmp);
       //xtmp= new char(memcounter); 
       tmp=(char*)realloc(tmp,memcounter);
       //tmp=strcpy(tmp,stmp);
       DEBUG("nach realloc: __%s__\nMemcounter fuer Realloc: %i\n",tmp,memcounter);
       restspeicher=memcounter;
        pagemul++;
     }

        DEBUG(" Loop: Vergleiche sf %c mit aktuellem Zeichen %c an Position %i[%c\%d]\n",*sf_buffer,*ot_buffer,t_buffer,tmp[t_buffer],tmp[t_buffer]);

      if(*sf_buffer!=*ot_buffer){break;} 

      DEBUG(" Zeichen %c passt mit %c\n",*ot_buffer,*sf_buffer);
      sf_counter++; 
      if(rt_counter++<=rt_len){tmp[t_buffer++]=*rt_buffer++;restspeicher_buffer--;}  
      *ot_buffer++;
      *sf_buffer++;
     }

    DEBUG("sf_counter  %i / sf_len %i\n",sf_counter,sf_len);

    if(sf_counter == sf_len){ 
                            DEBUG("  STRING PASST, bin noch an TMP-Pos %i\n",t_buffer);
			    //evtl Rest anfuegen
                            if(*rt_buffer){
				//t_buffer--;//test
			      while(*rt_buffer){

DEBUG("               aktueller String: %s\n",tmp);


				 DEBUG("    setze Restzeichen %c in TMP ein an Pos %i\n",*rt_buffer,t_buffer);

                                 tmp[t_buffer++]=*rt_buffer++;restspeicher_buffer--;}
				//Ende While
                              t_buffer++;   
                                          }
                            t=t_buffer;ot=ot_buffer;t--;
                            restspeicher=restspeicher_buffer;
                            }
                             else{tmp[t]=0;}

}//while buffer

DEBUG("TMP-Memory Zeichencounter an Position %i, aktuelles Zeichen in tmp-1: %c [%d]\n",t,tmp[t-1],tmp[t-1]);
tmp[t++]=*ot++;restspeicher--; 
tmp[t]=0;//Folgezeichen IMMER NULL
}//while SF

tmp[t]=0;
DEBUG("RUECK-STRING: %s\n",tmp);
qs_stop();
return tmp;
}

/*
packt einen String in eine Liste getrennt durch sep, das aus
einem Zeichen bestehen soll (deswegen explodec und nicht explode)
Rueckgabe = Vektor mit den Strings 

vector <string> ergebnis;
sep=" ";
suchtext="das ist ein test";
ergebnis=explodec(sep,suchtext);

int z=0;
for(z=0;z<ergebnis.size();z++)
{
cout <<  ergebnis[z];
cout <<"\n";
}


*/

vector<string>  xbkstringutil::explodec(string sep,string text)
{
qs_start();

register char **charvector;
register char *textzeiger=(char*)text.c_str();
register const char *sepzeiger=sep.c_str();
register int rueck=0;
register int z=0;
charvector = (char **)malloc(text.length());

vector<string> stringvector;

//Startpunkt = textzeiger
charvector[0]=&textzeiger[0];

while(*textzeiger) 
 {
 DEBUG("  textzeiger[%c]=sepzeiger[%c]?",*textzeiger,*sepzeiger);
 if(*textzeiger==*sepzeiger){
    rueck++;
    charvector[rueck]=textzeiger+1;//Startadresse
	textzeiger[0]=0;//das Zeichen an der Position 0 killen
	stringvector.push_back(charvector[rueck-1]);
	DEBUG("    Inhalt von charvector r-1 %i = %s",rueck-1,charvector[rueck-1]);

    }//if 
 *textzeiger++;
 }//while
if(rueck>0){stringvector.push_back(charvector[rueck]);}
qs_stop();

return stringvector;
}



/*
Wandelt einen URL codierten String in "normal" um
*/

string  xbkstringutil::URLDecode(string text)
{
qs_start();
register char *tmp = new char(text.length());
register const char *src = text.c_str();
register char hex[3];
register int x=0;
hex[2]=0;
 while(*src)
 {
  DEBUG("aktuelles Zeichen: %c",*src);
 if (*src=='%')
  {
   DEBUG("PROZENT gefunden");
   src++;
   DEBUG("h0 %c h1 %c",*(src),*(src+1));
   hex[0]=*src++;hex[1]=*src++; 
   DEBUG("hx0 %c hx1 %c",hex[0],hex[1]);
   tmp[x]=(char)strtol(hex, NULL, 16);
   DEBUG("Pos x: %i, Hex: %c",x,tmp[x]);
   DEBUG(" ");
  }
 else if (*src=='+')
  {
   tmp[x]=' ';*++src; 
  }
 else
 {
   tmp[x]=*src++; 
 }
 x++;
 }
qs_stop();
return tmp;
}


/*
Encodiert einen URL String
*/

string  xbkstringutil::URLEncode(string text)
{
register char *tmp = new char(text.length()*3);//eigentlich reicht auch *2
register const char *src = text.c_str();
register int x=0;
unsigned char hex[]="0123456789ABCDEF";
while(*src)
 {
 if(!isalnum(*src) && strchr("_-.",*src) == NULL)
  {
    DEBUG("Sonderzeichen %c gefunden",*src);
    tmp[x++]='%';
    tmp[x++]=hex[*src >> 4];//shr 4 Bits nach rechts fuer ersten Teil 
    tmp[x]=hex[*src & 15];//AND 0x0F also nur 0-F durchlassen 
   
    /*
    Ein Zeichen besteht aus 8 Bits, nehmen wir mal 11001100
    11001100 SHR 4 = 4 Bits nach rechts schieben 
    00001100 die Bits werden nicht rotated so wie ich das sehe
    so jetzt habe ich den linken Teil fuer den Array
    und jetzt AND 0x0F fuer den rechten Teil:
     11001100 AND 0xf
     00001111
    =00001100
    */


  }
 else if (*src==' ') { tmp[x]='+';}
 else
  {
  tmp[x]=*src;
  }
 *++src;++x;
 }

}
#endif

