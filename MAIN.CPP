 #include<dos.h>
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<process.h>

#include"d:\manishmouse.h"
#include"e:\softwa~1\cprogr~1\graphics.cpp"

void main()
  {

     GRAPHICS();

     char msg[20];
     int press=0,mx,my;

     int maxx=getmaxx();
     int maxy=getmaxy();

     setcolor(LIGHTBLUE);
     setfillstyle(INTERLEAVE_FILL,LIGHTBLUE);
     bar(0,0,maxx,maxy-21);

     BUTTON(0,maxy-21,maxx,maxy);
     BUTTON(3,maxy-19,70,maxy-1);

     outtextxy(6,maxy-12,"Start");
     SYSTEMTRAY(maxx-225,maxy-19,maxx-1,maxy-1);
     setcolor(BLACK);
     outtextxy(maxx-220,maxy-12,"Developed by Manish:Jyoti");
     setcolor(YELLOW);
     settextstyle(TRIPLEX_FONT,0,5);
     outtextxy(maxx/2-175,maxy/2-50,"An Antivirus Utility");
     showmouseptr();
     changemouseptr();

     settextstyle(0,0,1);

     while(1)
       {
	  getmousepos(&press,&mx,&my);

	  if(press == 1)
	    {
	      press = 0;
		if(mx>3 && mx<70)
		   if(my>maxy-19 && my<maxy-1)
		     {
		       PRESS_BUTTON(3,maxy-19,70,maxy-1);
		       ICONS();
		     }
	    }
	  if(press == 2)
	    {
	      closegraph();
	      exit(0);
	    }

       }

  }