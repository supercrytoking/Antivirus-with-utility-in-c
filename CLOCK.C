 #include"dos.h"

void interrupt (*prevtimer)();
void interrupt mytimer();

int running=0;

void writestring(char *str,int row, int col,int attb);
unsigned long far *time=(unsigned long far*)0x46C;

char far *scr;
char far *mode;

void  writechar(char ch,int row,int col,int attr)
      {
	*(scr+row*160+col*2)=ch;
	*(scr+row*160+col*2+1)=attr;
     }

void main()
  {
    if((*mode & 0x30)==0x30)
       scr=(char far*)0xB0000000;
    else
       scr=(char far*)0xB8000000;

       prevtimer=getvect(8);
       setvect(8,mytimer);
       keep(0,1000);
 }


void interrupt mytimer()
  {
    unsigned char hours,sec,min;

    if(running==0)
      {
	running=1;
	hours=(*time/65520);
	min=(*time-hours*65520)/1092;
	sec=(*time-hours*65520-min*1092)*10/182;

	if(sec>=60)
	  {
	    sec-=60;
	    min++;
	    if(min==60)
	      {
		min=0;
		hours++;
		if(hours==24)
		  hours=0;
	      }
	  }
	writestring(" JYOTI. ",2,70,82);
	writechar(48+hours/10,1,70,110);
	writechar(48+hours%10,1,71,110);
	writechar(':',1,72,110);
	writechar(48+min/10,1,73,110);
	writechar(48+min%10,1,74,110);
	writechar(':',1,75,110);
	writechar(48+sec/10,1,76,110);
	writechar(48+sec%10,1,77,110);

	running=0;
      }
      (*prevtimer)();
    }

//Writes a string at a given location on screen in given

//attributes

void writestring(char *str,int row, int col,int attb)

  {

    while(*str)
      {

	writechar(*str,row,col,attb);
	str++;
	col++;

     }

 }
