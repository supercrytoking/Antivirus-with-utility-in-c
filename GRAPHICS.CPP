#include<graphics.h>
#include<iostream.h>
#include<bios.h>

#include<stdio.h>

unsigned int size;
void *buffer;
int h;

char boot[512];
char bootfile[512];
char boottemp[512];
char part[512];
char partfile[512];
char parttemp[512];

FILE *bootbackup;
FILE *parttable;

char temp[2];
int val,drive;
int choice;
int nomatch=0;

void GRAPHICS()
  {
    int driver=DETECT,mode;
    initgraph(&driver,&mode,"e:\\softwa~1\\langua~1\\tc\\bgi");
    cleardevice();
  }

void BUTTON(int x1,int y1,int x2,int y2 )
{
 setfillstyle(SOLID_FILL,LIGHTGRAY);
 bar (x1,y1,x2,y2);
 //3-D Effect
 setcolor(WHITE);
 line(x1,y1,x1,y2);
 line(x1,y1,x2,y1);
 setcolor(BLACK);
 line(x2,y1,x2,y2);
 line(x1,y2,x2,y2);
}

/***************************************************/
void PRESS_BUTTON(int x1,int y1,int x2,int y2)
{
 hidemouseptr();
 setcolor(BLACK);
 line(x1,y1,x1,y2);
 line(x1,y1,x2,y1);
 setcolor(WHITE);
 line(x2,y1,x2,y2);
 line(x1,y2,x2,y2);

 delay(220);

 setcolor(WHITE);
 line(x1,y1,x1,y2);
 line(x1,y1,x2,y1);
 setcolor(BLACK);
 line(x2,y1,x2,y2);
 line(x1,y2,x2,y2);

 showmouseptr();
}

void ICONS()
   {

     char *a[] = { "Boot Backup",
		   "BOOT Virus Detect",
		   "Boot Recovery",
		   "Part Tab Backup",
		   "Part Detection",
		   "Part Tab Recovery",
		   "EXIT"
		  };

     int x=50,y=160;
     int x1=210,y1=190;
     int i=0,j=0;
     int k=6;
     int butt=0,mx,my;

     setfillstyle(SOLID_FILL,RED);
     setcolor(RED);
     bar(0,313,20,479-20);

     setcolor(BLACK);

     int xrad=25,yrad=35,sangle=0,eangle=360;

     settextstyle(0,0,1);

     for(int m= 0;m<=6;m++)
       {
	 BUTTON(20,479-40-j,200,479-20-j);
	 outtextxy(30,479-30-j,a[k]);
	 k--;
	 j+=21;
	 i+=50;
       }

      x=getmaxx();
      y=getmaxy();

      setcolor(YELLOW);
      outtextxy(x/2+150,y/2+200,"Progress Bar");
      setcolor(BLACK);
      rectangle(x/2+100,y/2+70,x/2+300,y/2+180);
      setcolor(LIGHTGRAY);
      bar(x/2+101,y/2+71,x/2+299,y/2+179);
      setcolor(BLACK);

      rectangle(x/2+109,y/2+79,x/2+161,y/2+161);
      setcolor(GREEN);                         //IDLE
      setfillstyle(SOLID_FILL,GREEN);
      bar(x/2+110,y/2+80,x/2+160,y/2+160);

      setcolor(BLACK);
      rectangle(x/2+229,y/2+79,x/2+281,y/2+161);
      setcolor(LIGHTGRAY);                     //BUSY
      setfillstyle(SOLID_FILL,LIGHTGRAY);
      bar(x/2+230,y/2+80,x/2+280,y/2+160);

      setcolor(YELLOW);
      outtextxy(x/2+115,y/2+165,"IDLE");
      outtextxy(x/2+245,y/2+165,"BUSY");

      while(1)
	{
	  getmousepos(&butt,&mx,&my);

	  if(butt == 1)
	    {
	      butt = 0;
		 if(mx>20 && mx<200)                //EXIT
		    if(my>439 && my<459)
		      {
			 PRESS_BUTTON(20,439,200,459);
			 closegraph();
			 exit(0);
		      }
		 if(mx>20 && mx<200)                //RECOVERY
		    if(my>418 && my<438)
		      {
			 PRESS_BUTTON(20,418,200,438);

			 size=imagesize(199,199,421,301);

			 buffer=malloc(size);
			 getimage(199,199,421,301,buffer);

			 BUTTON(200,200,420,225);

			 setcolor(BLACK);
			 outtextxy(205,210,"¯Recovering Part Table");
			 moveto(395,210);
			 outtext("|X|");

			 rectangle(200,241,420,300);

			 setcolor(WHITE);
			 setfillstyle(SOLID_FILL,WHITE);
			 bar(201,226,420,299);

			 setcolor(LIGHTGRAY);                         //IDLE
			 setfillstyle(SOLID_FILL,LIGHTGRAY);
			 bar(x/2+110,y/2+80,x/2+160,y/2+160);

			 setcolor(GREEN);                     //BUSY
			 setfillstyle(SOLID_FILL,GREEN);
			 bar(x/2+230,y/2+80,x/2+280,y/2+160);


			 setcolor(YELLOW);
			 setfillstyle(SOLID_FILL,YELLOW);

			 int j=0;

			 while(j!=215)
			    {
			      bar(205,270,205+j,290);
			      delay(300);
			      j+=5;
			    }

			  setcolor(BLACK);

			  moveto(205,228);
			  outtext("Press ESC TO EXIT");
			  moveto(205,238);
			  outtext("Recovery Complete!!!:");

			 while(1)
			   {
			     getmousepos(&butt,&mx,&my);
			     if(butt==1)
			       {
				 butt=0;
				   if(mx>395 && mx<420)
				     if(my>200 && my<300)
				       {
					 break;
				       }
			       }
			      if(kbhit())
				break;
			   }

			 putimage(199,199,buffer,COPY_PUT);

			 free(buffer);

			 setcolor(GREEN);                         //IDLE
			 setfillstyle(SOLID_FILL,GREEN);
			 bar(x/2+110,y/2+80,x/2+160,y/2+160);

			 setcolor(LIGHTGRAY);                     //BUSY
			 setfillstyle(SOLID_FILL,LIGHTGRAY);
			 bar(x/2+230,y/2+80,x/2+280,y/2+160);

		      }
		 if(mx>20 && mx<200)                //DETECTION
		    if(my>397 && my<417)
		      {
			 PRESS_BUTTON(20,397,200,417);


			 size=imagesize(199,199,421,301);

			 buffer=malloc(size);
			 getimage(199,199,421,301,buffer);

			 BUTTON(200,200,420,225);

			 setcolor(BLACK);
			 outtextxy(205,210,"¯Detecting Part Table");
			 moveto(395,210);
			 outtext("|X|");

			 rectangle(200,241,420,300);

			 setcolor(WHITE);
			 setfillstyle(SOLID_FILL,WHITE);
			 bar(201,226,420,299);

			 setcolor(LIGHTGRAY);                         //IDLE
			 setfillstyle(SOLID_FILL,LIGHTGRAY);
			 bar(x/2+110,y/2+80,x/2+160,y/2+160);

			 setcolor(GREEN);                     //BUSY
			 setfillstyle(SOLID_FILL,GREEN);
			 bar(x/2+230,y/2+80,x/2+280,y/2+160);


			 setcolor(YELLOW);
			 setfillstyle(SOLID_FILL,YELLOW);

			 int j=0;

			 while(j!=215)
			    {
			      bar(205,270,205+j,290);
			      delay(300);
			      j+=5;
			    }

			 biosdisk(2,128,0,0,1,1,parttemp);
			 parttable=fopen("c:\\windows\\system\\part.txt","rb");

			 for(i=0;i<512;i++)
			    {
			      partfile[i]=fgetc(parttable);
			    }

			 fclose(parttable);

			 nomatch=0;

			 for(i=0;i<512;i++)
			    {
			      if(parttemp[i]!=partfile[i])
				 nomatch++;
			    }

			  setcolor(BLACK);

			 if(nomatch>0)
				{
				  moveto(205,228);
				  outtext("Press ESC TO EXIT");
				  moveto(205,238);
				  outtext("WARNING :");
				  moveto(205,248);
				  outtext("A VIRUS HAS BEEN DETECTED");
				}

			  else
				{
				  moveto(205,228);
				  outtext("Press ESC TO EXIT");
				  moveto(205,238);
				  outtext("Yeah!");
				  moveto(205,248);
				  outtext("NO VIRUS HAS BEEN DETECTED");
				}


			 while(1)
			   {
			     getmousepos(&butt,&mx,&my);
			     if(butt==1)
			       {
				 butt=0;
				   if(mx>395 && mx<420)
				     if(my>200 && my<300)
				       {
					 break;
				       }
			       }
			      if(kbhit())
				break;
			   }

			 putimage(199,199,buffer,COPY_PUT);

			 free(buffer);

			 setcolor(GREEN);                         //IDLE
			 setfillstyle(SOLID_FILL,GREEN);
			 bar(x/2+110,y/2+80,x/2+160,y/2+160);

			 setcolor(LIGHTGRAY);                     //BUSY
			 setfillstyle(SOLID_FILL,LIGHTGRAY);
			 bar(x/2+230,y/2+80,x/2+280,y/2+160);

		      }
		 if(mx>20 && mx<200)                //PARTITION TABLE BACKUP
		    if(my>376 && my<396)
		      {
			 PRESS_BUTTON(20,376,200,396);

			 size=imagesize(199,19,421,261);

			 buffer=malloc(size);
			 getimage(199,19,421,261,buffer);

			 BUTTON(200,20,420,45);

			 setcolor(BLACK);
			 outtextxy(205,30,"¯Partition Table BackUp.");
			 moveto(390,30);
			 outtext("|X|");

			 setcolor(GREEN);
			 rectangle(200,46,420,260);

			 setcolor(WHITE);
			 setfillstyle(SOLID_FILL,WHITE);
			 bar(201,46,419,260);

			 setcolor(BLACK);
			 moveto(205,47);
			 outtext("Continue...(y/n)");
			 moveto(210,57);

			 while(1)
			  {
			    choice=getch();

			    if(drive==27||choice==78||choice==110) goto slide;

			    if(choice==89||choice==121) break;
			  }

			setcolor(LIGHTGRAY);                         //IDLE
			setfillstyle(SOLID_FILL,LIGHTGRAY);
			bar(x/2+110,y/2+80,x/2+160,y/2+160);

			setcolor(GREEN);                     //BUSY
			setfillstyle(SOLID_FILL,GREEN);
			bar(x/2+230,y/2+80,x/2+280,y/2+160);

			setcolor(LIGHTGRAY);                         //IDLE
			setfillstyle(SOLID_FILL,LIGHTGRAY);
			bar(x/2+110,y/2+80,x/2+160,y/2+160);
			setcolor(GREEN);                     //BUSY
			setfillstyle(SOLID_FILL,GREEN);
			bar(x/2+230,y/2+80,x/2+280,y/2+160);

			//read partition table sector

			biosdisk(2,128,0,0,1,1,part);

			parttable=fopen("C:\\Windows\\system\\part.txt","wb");

			for(h=0;h<512;h++)
			   putc(part[h],parttable);
			fclose(parttable);

			outtextxy(205,77,"Bootable 0x80 (0x00 if Not)");

			delay(500);

			outtextxy(205,90,"Starting head");

			delay(500);

			outtextxy(205,107,"Start Cylender");

			delay(500);

			outtextxy(205,120,"Partition Type");

			delay(500);

			outtextxy(205,137,"Ending head");

			delay(500);

			outtextxy(205,150,"Ending Cylender");

			delay(500);

			outtextxy(205,167,"Starting Sector");

			delay(500);

			outtextxy(205,180,"Start Sector Relative to");

			delay(500);

			outtextxy(205,197,"¯¯ disk beginning.");

			setcolor(GREEN);                         //IDLE
			setfillstyle(SOLID_FILL,GREEN);
			bar(x/2+110,y/2+80,x/2+160,y/2+160);

			setcolor(LIGHTGRAY);                     //BUSY
			setfillstyle(SOLID_FILL,LIGHTGRAY);
			bar(x/2+230,y/2+80,x/2+280,y/2+160);

			moveto(205,238);
			outtext("Backup Created...");
			moveto(205,248);
			outtext("c:\\windows\\parttab.txt");

			 while(1)
			   {
			     getmousepos(&butt,&mx,&my);
			     if(butt==1)
			       {
				 butt=0;
				   if(mx>390 && mx<420)
				     if(my>20 && my<45)
				       {
					 break;
				       }
			       }
			      if(kbhit())
				break;
			   }

	      slide:	putimage(199,19,buffer,COPY_PUT);
			free(buffer);

			 delay(1000);

			 setcolor(GREEN);                         //IDLE
			 setfillstyle(SOLID_FILL,GREEN);
			 bar(x/2+110,y/2+80,x/2+160,y/2+160);

			 setcolor(LIGHTGRAY);                     //BUSY
			 setfillstyle(SOLID_FILL,LIGHTGRAY);
			 bar(x/2+230,y/2+80,x/2+280,y/2+160);

		      }

		 if(mx>20 && mx<200)
		    if(my>355 && my<375)               //RECOVERY
		      {
			 PRESS_BUTTON(20,355,200,375);

			 size=imagesize(199,199,421,301);

			 buffer=malloc(size);
			 getimage(199,199,421,301,buffer);

			 BUTTON(200,200,420,225);

			 setcolor(BLACK);
			 outtextxy(205,210,"¯Recovering Boot Sector");
			 moveto(395,210);
			 outtext("|X|");

			 rectangle(200,241,420,300);

			 setcolor(WHITE);
			 setfillstyle(SOLID_FILL,WHITE);
			 bar(201,226,420,299);

			 setcolor(LIGHTGRAY);                         //IDLE
			 setfillstyle(SOLID_FILL,LIGHTGRAY);
			 bar(x/2+110,y/2+80,x/2+160,y/2+160);

			 setcolor(GREEN);                     //BUSY
			 setfillstyle(SOLID_FILL,GREEN);
			 bar(x/2+230,y/2+80,x/2+280,y/2+160);


			 setcolor(YELLOW);
			 setfillstyle(SOLID_FILL,YELLOW);

			 int j=0;

			 while(j!=215)
			    {
			      bar(205,270,205+j,290);
			      delay(300);
			      j+=5;
			    }

			  setcolor(BLACK);

			  moveto(205,228);
			  outtext("Press ESC TO EXIT");
			  moveto(205,238);
			  outtext("Recovery Complete!!!:");

			 while(1)
			   {
			     getmousepos(&butt,&mx,&my);
			     if(butt==1)
			       {
				 butt=0;
				   if(mx>395 && mx<420)
				     if(my>200 && my<300)
				       {
					 break;
				       }
			       }
			      if(kbhit())
				break;
			   }

			 putimage(199,199,buffer,COPY_PUT);

			 free(buffer);

			 setcolor(GREEN);                         //IDLE
			 setfillstyle(SOLID_FILL,GREEN);
			 bar(x/2+110,y/2+80,x/2+160,y/2+160);

			 setcolor(LIGHTGRAY);                     //BUSY
			 setfillstyle(SOLID_FILL,LIGHTGRAY);
			 bar(x/2+230,y/2+80,x/2+280,y/2+160);

		      }
		 if(mx>20 && mx<200)
		    if(my>334 && my<354)               //DETECTION
		      {
			 PRESS_BUTTON(20,334,200,354);


			 size=imagesize(199,199,421,301);

			 buffer=malloc(size);
			 getimage(199,199,421,301,buffer);

			 BUTTON(200,200,420,225);

			 setcolor(BLACK);
			 outtextxy(205,210,"¯Detecting BOOT SECTOR");
			 moveto(395,210);
			 outtext("|X|");

			 rectangle(200,241,420,300);

			 setcolor(WHITE);
			 setfillstyle(SOLID_FILL,WHITE);
			 bar(201,226,420,299);

			 setcolor(LIGHTGRAY);                         //IDLE
			 setfillstyle(SOLID_FILL,LIGHTGRAY);
			 bar(x/2+110,y/2+80,x/2+160,y/2+160);

			 setcolor(GREEN);                     //BUSY
			 setfillstyle(SOLID_FILL,GREEN);
			 bar(x/2+230,y/2+80,x/2+280,y/2+160);


			 setcolor(YELLOW);
			 setfillstyle(SOLID_FILL,YELLOW);

			 int j=0;

			 while(j!=215)
			    {
			      bar(205,270,205+j,290);
			      delay(300);
			      j+=5;
			    }

			 biosdisk(2,128,0,0,1,1,parttemp);
			 parttable=fopen("c:\\windows\\system\\part.txt","rb");

			 for(i=0;i<512;i++)
			    {
			      partfile[i]=fgetc(parttable);
			    }

			 fclose(parttable);

			 nomatch=0;

			 for(i=0;i<512;i++)
			    {
			      if(parttemp[i]!=partfile[i])
				 nomatch++;
			    }

			  setcolor(BLACK);

			 if(nomatch>0)
				{
				  moveto(205,228);
				  outtext("Press ESC TO EXIT");
				  moveto(205,238);
				  outtext("WARNING :OUCH !!!");
				  moveto(205,248);
				  outtext("A VIRUS HAS BEEN DETECTED");
				}

			  else
				{
				  moveto(205,228);
				  outtext("Press ESC TO EXIT");
				  moveto(205,238);
				  outtext("Yeah!");
				  moveto(205,248);
				  outtext("NO VIRUS HAS BEEN DETECTED");
				}


			 while(1)
			   {
			     getmousepos(&butt,&mx,&my);
			     if(butt==1)
			       {
				 butt=0;
				   if(mx>395 && mx<420)
				     if(my>200 && my<300)
				       {
					 break;
				       }
			       }
			      if(kbhit())
				break;
			   }

			 putimage(199,199,buffer,COPY_PUT);

			 free(buffer);

			 setcolor(GREEN);                         //IDLE
			 setfillstyle(SOLID_FILL,GREEN);
			 bar(x/2+110,y/2+80,x/2+160,y/2+160);

			 setcolor(LIGHTGRAY);                     //BUSY
			 setfillstyle(SOLID_FILL,LIGHTGRAY);
			 bar(x/2+230,y/2+80,x/2+280,y/2+160);

		      }
		 if(mx>20 && mx<200)
		    if(my>313 && my<333)               //BOOTBACKUP
		      {
			 PRESS_BUTTON(20,313,200,333);

			 size=imagesize(199,19,421,261);

			 buffer=malloc(size);
			 getimage(199,19,421,261,buffer);

			 BUTTON(200,20,420,45);

			 setcolor(BLACK);
			 outtextxy(205,30,"¯¯BootSector BackUp...");
			 moveto(390,30);
			 outtext("|X|");

			 setcolor(GREEN);
			 rectangle(200,46,420,260);

			 setcolor(WHITE);
			 setfillstyle(SOLID_FILL,WHITE);
			 bar(201,46,419,260);

			 setcolor(BLACK);
			 moveto(205,47);
			 outtext("Enter drive: ESC to Exit");
			 moveto(210,57);
			 outtext("(0 = A,2 = C)");
			 while(1)
			  {
			    drive=getch();

			    if(drive==27) goto skip;

			    if(drive==65||drive==67)
			      {
				 drive-=65;
				 break;
			      }
			    if(drive==97||drive==99)
			      {
				 drive-=97;
				 break;
			      }
			    if(drive==48||drive==50)
			      {
				 drive-=48;
				 break;
			      }
			}

			setcolor(LIGHTGRAY);                         //IDLE
			setfillstyle(SOLID_FILL,LIGHTGRAY);
			bar(x/2+110,y/2+80,x/2+160,y/2+160);
			setcolor(GREEN);                     //BUSY
			setfillstyle(SOLID_FILL,GREEN);
			bar(x/2+230,y/2+80,x/2+280,y/2+160);

			//read boot sector

			absread(drive,1,0,boot);

			bootbackup=fopen("C:\\Windows\\system\\boot.txt","wb");

			for(h=0;h<512;h++)
			   putc(boot[h],bootbackup);
			fclose(bootbackup);

			outtextxy(205,77,"System_ID");

			delay(500);

			outtextxy(205,90,"Bytes per SEC");

			delay(500);

			outtextxy(205,107,"Sec per Clus");

			delay(500);

			outtextxy(205,120,"FAT Copies");

			delay(500);

			outtextxy(205,137,"RES SEC");

			delay(500);

			outtextxy(205,150,"Root_Dir_Entry");

			delay(500);

			outtextxy(205,167,"NO_SEC");

			delay(500);

			outtextxy(205,180,"Format_ID");

			delay(500);

			outtextxy(205,197,"Sec_Per_FAT");

			delay(500);

			outtextxy(205,210,"NO_SIDES  :  2");

			delay(1000);

			setcolor(GREEN);                         //IDLE
			setfillstyle(SOLID_FILL,GREEN);
			bar(x/2+110,y/2+80,x/2+160,y/2+160);

			setcolor(LIGHTGRAY);                     //BUSY
			setfillstyle(SOLID_FILL,LIGHTGRAY);
			bar(x/2+230,y/2+80,x/2+280,y/2+160);

			moveto(205,238);
			outtext("Backup Created...");
			moveto(205,248);
			outtext("c:\\windows\\boottab.txt");

			 while(1)
			   {
			     getmousepos(&butt,&mx,&my);
			     if(butt==1)
			       {
				 butt=0;
				   if(mx>390 && mx<420)
				     if(my>20 && my<45)
				       {
					 break;
				       }
			       }
			      if(kbhit())
				break;
			   }

	      skip:	putimage(199,19,buffer,COPY_PUT);
			free(buffer);

		      }
	  }
      }
  }

void SYSTEMTRAY(int x1,int y1,int x2,int y2 )
{
 setfillstyle(SOLID_FILL,LIGHTGRAY);
 bar (x1,y1,x2,y2);
 //3-D Effect
 setcolor(BLACK);
 line(x1,y1,x1,y2);
 line(x1,y1,x2,y1);
 setcolor(WHITE);
 line(x2,y1,x2,y2);
 line(x1,y2,x2,y2);
}