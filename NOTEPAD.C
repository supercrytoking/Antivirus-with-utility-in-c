 /**************************************************************************/
/**************************************************************************/
/*****                         A TSR NOTEPAD			      *****/
/*****                                                                *****/
/*****                        A MAJOR PROJECT                         *****/
/*****                                                                *****/
/*****                    DEVELOPED AND COMPILED BY                   *****/
/*****                                                                *****/
/*****                      ALL RIGHTS RESERVED                       *****/
/**************************************************************************/
/**************************************************************************/

#include                      "stdio.h"

#include                      "alloc.h"

#include                      "conio.h"

#include                      "io.h"

#include                      "string.h"

#include                      "fcntl.h"

#include                      "stat.h"

#include                      "dos.h"


#define   ESC                 0x1B

#define   ALT                 8

#define   N                   49

#define   TAB                 9

#define   UP                  72

#define   DOWN                80

#define   LEFT                75

#define   RIGHT               77

#define   DEL                 83

#define   INSERT              128


#define F1 59
#define F2 60              //    NOTEPAD
#define F3 61              //   FUNCTIONS
#define F4 62

#define F5 63              //   ASCII TABLE
#define F6 64              // POP UP CALENDER

#define F7 65              //Help Readme
#define MAX 1794             //Total Capacity of file 24*79=1794 characters

void interrupt (*prev_8)();
void interrupt (*prev_9)();
void interrupt (*prev_28)();

void interrupt (*prev_1b)();
void interrupt (*prev_23)();

void interrupt our_8();
void interrupt our_9();
void interrupt our_28();

void interrupt our_1b();
void interrupt our_23();


union REGS i,o;
struct SREGS s;

int r,c,k;
char far *scr=(char far*) 0xB8000000L,*buffer;



//Writes a character at a given location on the screenin given

//attributes

void writechar(char ch,int row,int col,int attb)

  {

    *(scr+row*160+col*2)=ch;
    *(scr+row*160+col*2+1)=attb;

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

//Clears a window on screen in given attributes

void clrwin(int sr,int sc,int er,int ec,int attb)
  {

    i.h.ah=6;
    i.h.al=0;
    i.h.ch=sr;
    i.h.cl=sc;
    i.h.dh=er;
    i.h.dl=ec;
    i.h.bh=attb;

    int86(0x10,&i,&i);

  }

//Saves a window on screen

void savevideo(int sr,int sc,int er,int ec)

  {

    for(k=0,r=sr;r<=er;r++)

      {

	for(c=sc;c<=ec;c++)

	  {

	    //buffer is a globel pointer pointing to area using a call

	    //to malloc
	    buffer[k]=*(scr+r*160+c*2);
	    buffer[k+1]=*(scr+r*160+c*2+1);

	    k+=2;

	 }

     }

 }

//Restores a window on the screen

void restorevideo(int sr,int sc,int er,int ec)

  {

    for(k=0,r=sr;r<=er;r++)

      {

	for(c=sc;c<=ec;c++)

	  {

	    //buffer is a globel pointer pointing to area allocated

	    //using a call to malloc()

	    *(scr+r*160+c*2)=buffer[k];
	    *(scr+r*160+c*2+1)=buffer[k+1];

	    k+=2;

	  }

      }

  }

void getdosflag();
void getcriterror();
void capture_interrupts();
void activate_tsr();
void get_exterr();
void set_exterr();
void set_psp(unsigned newpsp);
void capture_ctrlbrk();
void release_ctrlbrk();
void cursor(int ssl,int esl);
void getcursor();
void getkey();
void getstring(char *gc,int size,int r,int c);
void notepad();
void insertchar(char ch);
int  lsize(int lb);
void display_error(char *msg);
void displayline(int l,int r);
void help();
void enter();
void left();
void right();
void up();
void down();
void backspace();
void del();
void save();
void load();
void new1();
void asciitab();
void calender();

void getname(char fn[],int size);
void displaybuffer();

unsigned want_to_popup,active,key,area,ss,sp,ext_err[3];
unsigned our_psp,foreground_psp;

int row,col,j,px,py,pssl,pesl,diskbusy;
int fsize,fpos,line_beg,nr=1,nc=1,er=1,ec=1;
int insert=1,ascii,scan,tspc;
int handle,mj,jm;

struct date datep;
int days[]={31,28,31,30,31,30,31,31,30,31,30,31};
int thisyrdays,leapyears,firstday,x;
char *months[]={
		  "January","February","March","April",
		   "May","June","July","August",
		   "November","December"
	       };
long int totaldays;


unsigned char far *kb=(unsigned char far*)0x417;
unsigned char far *dosbusy,far *error,far *foreground_dta;

char far (stack[4000]),filename[67]="NONAME00.TMP";
char tempfile[67],notepad_buffer[MAX],outstr[10];

FILE *fp;

void main()

  {

     for(mj=5;mj<=17;mj++)
	for(jm=15;jm<=68;jm++)
	    writechar('±',mj,jm,17);

     writestring("An Application of TSR :NOTEPAD",5,25,23);
     writestring("Developed by :Manish and Jyoti",7,16,23);
     writestring("N.C.College Of Engg.(Israna)Panipat.",9,17,23);
     writestring("Hot Key:<Alt><N>",14,25,23);
     writestring("NOTEPAD installed...",16,17,23);

    //store PSP of our program

    our_psp=getpsp();

//-----------------------------------------------------------------------

// HELP to  getpsp()

//-----------------------------------------------------------------------

// Gets the program segment prefix

// Declaration:  unsigned getpsp(void);

// Remarks:
//getpsp uses DOS call 0x62 to get the segment
//address of the PSP (program segment prefix).

//getpsp exists only in DOS 3.x. For versions of
//DOS 2.x and 3.x, you can use the global
//variable _psp set by the start-up code
//instead.

//Return Value:
//Returns the segment address of the PSP.
//------------------------------------------------------------------------

//Calculate and allocate memory required to save the window on screen

area=((24-0+1)*(79-0+1)*2);

buffer=malloc(area);

//If memeory is not available.....
if(buffer==NULL)

  printf("\n\nInsufficient memory,Aborting.....");

else

  {
    //get address of DOS busy flag
    getdosflag();

    //get address of critical error flag
    getcriterror();

    //Capture interrupts
    capture_interrupts();

    //terminate but stay resident
    keep(0,3000);

  }

}

//get address of DOS busy flag

void getdosflag()

  {

    i.h.ah=0x34;
    intdosx(&i,&o,&s);

//-------------------------------------------------------------------------

//HELP intdosx()

//-------------------------------------------------------------------------

//intdos and intdosx execute DOS interrupt 0x21 to invoke a specified DOS
//function. The value of inregs->h.ah specifies the DOS function to be
//invoked.
//
//intdosx also copies the segregs ->ds and segregs ->es values into the
//corresponding registers before invoking the DOS function.
//-------------------------------------------------------------------------

    //get address of DOS busy flag

    dosbusy=(char far*)MK_FP(s.es,o.x.bx);

 }

//get address of critical error flag

void getcriterror()

 {

   i.h.ah=0x5D;
   i.h.al=6;
   intdosx(&i,&o,&s);

   //get address of critical error flag

   error=MK_FP(s.ds,o.x.si);

 }

//Capture interrupts

void capture_interrupts()

  {

    //Capture timer interrupt

    prev_8=getvect(8);
    setvect(8,our_8);

    //Capture Keyboard interrupt
    prev_9=getvect(9);
    setvect(9,our_9);

    //capture disk I/O interrupt
//    prev_13=getvect(0x13);
//    setvect(0x13,our_13);

    //capture dos idle interrupt

    prev_28=getvect(0x28);
    setvect(0x28,our_28);

  }

//ISR for timer interrupt
void interrupt our_8()

  {

    (*prev_8)();

    //if popup request has been made
    //and TSR is not already active
    //and DOS is not busy
    //and DOS is not processing critical error
    //and no disk I/o is taking place
    if(want_to_popup==1 && active==0 &&*dosbusy==0 &&*error==0 &&diskbusy==0)
    activate_tsr();

 }


//ISR for keyboard interrupt

void interrupt our_9()

  {

    key=inportb(0x60);
    //if TSR not already active and Alt+N is pressed
    //make popup request
    if(key==N &&(*kb&ALT)==ALT &&active==0)
      want_to_popup=1;
    (*prev_9)();

  }

//ISR for DOS idle interrupt

void interrupt our_28()

  {

    (*prev_28)();
    //if popup request has been made
    //and TSR is not already active
    //DOS is not prompting critical tasks
    //DOS is not processing critical errors
    //and no disk I/O is taking place
    if(want_to_popup==1 &&active==0 && *dosbusy<=1 && *error==0
       &&diskbusy==0)
    activate_tsr();

  }

//activate notepad

void activate_tsr()

  {

    active=1;

    //switch to new stack
    //store the address of stack of foreground process
    //store current values of SS and SP

    ss=_SS;
    sp=_SP;

    //disable interrupts

    disable();

    //store segment:offset of new stack in SS:SP

    _SS=FP_SEG(stack);
    _SP=FP_OFF(stack+3999);

    //enable interrupts

    enable();

    //save foreground program's PSP and set TSR's PSP

    foreground_psp=getpsp();
    set_psp(our_psp);

    //save foreground program's DTA and set new DTA

    foreground_dta=getdta();
    setdta(MK_FP(our_psp,0x80));

    //capture Ctrl+C and Ctrl+Break temporarily

    capture_ctrlbrk();

    //save current cursor position and size

    px=wherex();
    py=wherey();
    getcursor();

    //start notepad

    notepad();

    //restore cursor position and size

    gotoxy(px,py);
    cursor(pssl,pesl);

    release_ctrlbrk();


    //restore foreground program's DTA

    setdta(foreground_dta);

    //restore foreground program's PSP

    set_psp(foreground_psp);

    //switch back to foreground program's stack

    disable();
    _SS=ss;
    _SP=sp;
    enable();

    //make active and want_to_popup 0 to indicate that TSR

    //is not active now
    active=0;
    want_to_popup=0;

 }


//sets psp

void set_psp(unsigned newpsp)
  {
    i.h.ah=0x50;
    i.x.bx=newpsp;
    intdos(&i,&o);

//----------------------------------------------------------------------

//HELP intdos()

//----------------------------------------------------------------------


// Remarks:
//intdos and intdosx execute DOS interrupt 0x21 to invoke a specified DOS
//function. The value of inregs->h.ah specifies the DOS function to be
//invoked.
//
//intdosx also copies the segregs ->ds and segregs ->es values into the
//corresponding registers before invoking the DOS function.
//
//This feature lets programs that use far pointers or a large data memory
//model specify which segment is to be used for the function execution.
//
//With intdosx, you can invoke a DOS function that takes a value of DS
//different from the default data segment and/or takes an argument in ES.
//
// After the interrupt
// -------------------
// After the interrupt 0x21 returns, these functions copy the following:
// þ current register values          to outregs
// þ status of the carry flag         to the x.cflag field in outregs
// þ value of the 8086 flags register to the x.flags field in outregs
//
//intdosx also sets the segregs->es and segregs->ds fields to the values of
//the corresponding segment registers and then restores DS.

//If the carry flag is set, it indicates that an error occurred.

//þ NOTE: inregs can point to the same structure that outregs points to.

// Return Value:
//Both functions return the value of AX after completion of the DOS function
//call.

//If the carry flag is set, indicating an error (outregs -> x.cflag != 0),
//these functions set _doserrno to the error code.

//-------------------------------------------------------------------------

  }

//captures Ctrl-C and Ctrl-Break intrrupts

void capture_ctrlbrk()

  {

    prev_1b=getvect(0x1b);
    setvect(0x1b,our_1b);
    prev_23=getvect(0x23);
    setvect(0x23,our_23);

 }

//restores Ctrl-C and Ctrl-BreaK interrupts

void release_ctrlbrk()

  {

    setvect(0x1b,prev_1b);
    setvect(0x23,prev_23);

  }

//ISR for Ctrl-Break interrupt

void interrupt our_1b()

  {

      ;

  }

//ISR for Ctrl-C interrupt

void interrupt our_23()

  {

      ;

  }

//sets cursor size

void cursor(int ssl,int esl)

  {

    i.h.ah=1;
    i.h.ch=ssl;
    i.h.cl=esl;
    int86(0x10,&i,&o);

  }

//gets cursor size

void getcursor()

   {

     i.h.ah=3;
     i.h.bh=0;
     int86(0x10,&i,&o);

     pssl=o.h.ch;
     pesl=o.h.cl;

  }

//gets a keystroke and stores its ascii,scan code

void getkey()

  {

    key=bioskey(0);

//-----------------------------------------------------------------------

//HELP bioskey()

//-----------------------------------------------------------------------

// Keyboard interface, using BIOS services directly
//
// Declaration:
//  þ int bioskey(int cmd);
//  þ unsigned _bios_keybrd(unsigned cmd);
//
// Remarks:
//bioskey and _bios_keybrd perform various keyboard operations using BIOS
//interrupt 0x16.
//
//The parameter cmd determines the exact keyboard operation performed.
//
// Return Value:
//The return value depends on the keyboard task performed.
//-------------------------------------------------------------------------
    //seperate ascii and scan codes of key hit
    ascii=(key&0x00ff);
    scan=(key>>8);

 }

//accepts a string from keyboard

void getstring(char *gc,int size,int r,int c)

  {

    for(j=0;j<size-1;j++)

      {

	gotoxy(c,r);
	getkey();
	if(ascii==0)

	  {

	    j--;
	    continue;

	  }
       if(ascii=='\b')

	 {

	   if(j==0)

	     {

	       j--;

	     }
	   c--;
	   j-=2;
	   writechar(' ',r-1,c-1,112);
	   continue;

	 }
       if(ascii=='\r')
	 break;

       gc[j]=toupper(ascii);
       writechar(ascii,r-1,c-1,112);

       c++;

     }
   gc[j]='\0';

 }

//notepad function

void notepad()

  {

    //saves screen

    savevideo(0,0,24,79);

    //clears screen in reverse video attribute
    clrwin(0,0,24,79,112);

    //turn insert on

    *kb|=INSERT;

    //show status line and menu

    for(j=0;j<80;j++)

      {

	writechar(' ',0,j,71);
	writechar(' ',24,j,71);

      }

   writestring("INSERT",24,73,71);
   writestring(filename,0,1,71);
   writestring("F2 Save   F3 Load   F4 New   Esc Exit",24,1,71);

   //display contents of buffer

   displaybuffer();

   //normal cursor is shown when Insert is ON

   cursor(6,7);

   while(1)

     {

       //position cursor at appropriate row and column

       gotoxy(nc+1,nr+1);

       //show current row and column position at status line

       sprintf(outstr,"%02u %02u",nr,nc);
       writestring(outstr,0,70,71);

       //get a keystroke

       getkey();

       //check status of insert line

       if((*kb&INSERT)==INSERT)

	 {

	   if(insert==0)

	     {

	       cursor(6,7);
	       writestring("INSERT",24,73,71);

	     }

	  insert=1;

	}

      else

	{

	  if(insert==1)

	    {

	      cursor(0,7);
	      writestring("     ",24,73,71);

	    }
	  insert=0;

	}

      //if ascii code is 0,check scan code

      if(ascii==0)

	{

	  switch(scan)

	    {

	      case UP:
		 up();
		 break;

	      case DOWN:
		  down();
		  break;

	     case LEFT:
		  left();
		  break;

	     case RIGHT:
		  right();
		  break;

	     case DEL:
		  del();
		  break;

	     case F1:
		  help();
		  break;

	     case F2:
		  save();
		  break;

	     case F3:
		  load();
		  break;

	     case F4:
		  new1();
		  break;

	     case F5:
		  asciitab();
		  break;

	     case F6:
		  calender();
		  break;

	   }

	}

	else

	  {

	    switch(ascii)

	       {

		 case '\b':
		       backspace();
		       break;

		 case '\r':
		       enter();
		       break;

		 case TAB:

		      //determines next tab stop and add appropriate

		      //number of spaces

		      tspc=5-(nc%5);
		      for( ;tspc;tspc--)
			insertchar(' ');
			displayline(line_beg,nr);
		      break;

		 case ESC:

		      //Return Control to Foreground Process

		      restorevideo(0,0,24,79);
		      return;

		 default:
		      insertchar(ascii);

	     }

	  }

       }

    }

//inserts the character just typed into the file

void insertchar(char ch)

  {

     //insert is on

     //or current buffer locations contains '\n'

     //or current buffer is beyond the lase character in the buffer

     if(insert==1 ||notepad_buffer[fpos]=='\n' ||fsize==fpos)

       {

	 //file size reached maximum or we are at the bottom right
	 //corner of the notepad window

	 if(fsize==MAX||(nr==23&&ec==77))

	   {

	     display_error("Buffer Full, Can't Continue...");
	     return;

	   }

	 //if line exceeds maximum number of characters in a line

	 if(lsize(line_beg)==77)

	   {

	     //if pointer points end of file,insert a carriage return

	     if(fsize==fpos)

	       {

		 display_error("Line too long, CR inserted.");
		 enter();

	       }

	    else

	      {

		display_error("Can't insert any more characters.");
		return;

	      }

	   }

	 //move all characters after the current location in
	 //the buffer forward by one character

	 memmove(notepad_buffer+fpos+1,notepad_buffer+fpos,fsize-fpos);

	 //insert the character just typed at current position
	 //in the buffer

	 notepad_buffer[fpos++]=ch;

	 //increment file size by 1

	 fsize++;

	 //display the entire line

	 displayline(line_beg,nr);

	 //increment current column position

	 nc++;

       }

      else

	{

	  //overwrite character present at current position
	  //in the buffer by character just typed

	  notepad_buffer[fpos++]=ch;

	  //writecharacter at current row and column and increment
	  //the current column position

	  writechar(ch,nr,nc++,112);

	}

	//if current line is the last line of the file,
	//increment ending column position

	if(nr==er)
	  ec++;

     }


//determines size of current line

int lsize(int lb)

  {

    for(j=0;notepad_buffer[lb+j]!='\n'&&lb+j<fsize;j++);
    return(j);

  }

//display error message

void display_error(char *msg)

  {

    //clear message window

    clrwin(10,20,15,59,77);

    //writeerror message

    writestring(msg,12,25,77);

    //wait for a keystroke

    writestring("Press any key...",13,25,77);
    getkey();

    //display the screen

    displaybuffer();

 }

//display the current line in the file

void displayline(int l,int r)

  {

    j=0;

    //until '\n' or end of file position is reached

    while(notepad_buffer[l+j]!='\n'&&(l+j)<fsize)

      {

	writechar(notepad_buffer[l+j],r,j+1,112);
	j++;

      }

   writechar(' ',r,j+1,112);

 }

//display help

void help()

  {

    int row,column,i;

    clrwin(5,10,19,70,17);

    for(i=10;i<=70;i++)
       writechar(' ',5,i,72);


    writestring("HELP MENU",5,35,23);
    writestring("F1                        HELP     |",7,12,23);
    writestring("F2                        SAVE     |      NOTEPAD",8,12,23);
    writestring("F3                        LOAD     |     FUNCTIONS",9,12,23);
    writestring("F4                        NEW      |",10,12,23);
    writestring("F5                        ASCII TABLE",11,12,23);
    writestring("F6                        CALENDER",12,12,23);
    writestring("F7 (WHEN ESCAPE NOTEPAD)  BACK UP OF PARTITION TABLE",
						   13,12,23);
    writestring("F8 (WHEN ESCAPE NOTEPAD)  CHECK FOR PARTITION TABLE VIRUS",
						   14,12,23);
    writestring("F9 (WHEN ESCAPE NOTEPAD)  RECOVERY OF PARTITION TABLE VIRUS",
						   15,12,23);
    writestring("F10(WHEN ESCAPE NOTEPAD)  BACK UP OF BOOT SECTOR",
						   16,12,23);
    writestring("F1 (WHEN ESCAPED NOTEPAD) CHECK FOR BOOT SECTOR VIRUS",
						   17,12,23);
    writestring("F2 (WHEN ESCAPED NOTEPAD) RECOVERY OF BOOT SECTOR VIRUS",
						   18,12,23);

    getkey();

    displaybuffer();

  }

//function to display ascii table

void asciitab()

  {

    int i,j,k=0,a;
    char value=0;
    char temp[20];

    clrwin(5,10,19,68,17);

    for(i=10;i<=68;i++)
       writechar(' ',5,i,72);
    writestring("ASCII TABLE",5,32,23);

    i=6;
    j=12;

    for(k=0;k<256;k++)

       {

	 itoa(k,temp,10);
	 writestring(temp,i,j,23);
	 j+=4;
	 writechar(value,i,j,23);
	 value++;

	 j-=4;
	 i++;

	 if(i==18)

	   {

	     i=6;
	     j+=12;

	       if(j>60)

		 {

		   writestring("Press any key to cont[Esc to Abort]",
				19,12,23);


		   getkey();
		   if(ascii==ESC)
		      break;

		   clrwin(5,10,19,68,17);
		   for(a=10;a<=68;a++)
			writechar(' ',5,a,72);
		   writestring("ASCII TABLE",5,32,23);

		   j=12;

		}

	  }

       }

       if(k==256)

	 {

	   writestring("Press any key to cont[Esc to Abort]",
			 19,12,23);
	   getkey();

	 }

	 displaybuffer();

   }

void calender()

  {

     int i,j,k,y,m;
     char temp[10],temp1[10];

     getdate(&datep);
     y=datep.da_year;
     m=datep.da_mon;

    clrwin(5,5,21,70,17);
    for(i=5;i<=70;i++)
       writechar(' ',5,i,72);
    writestring("CALENDER",5,32,23);

     do

       {

	 days[1]=28;
	 thisyrdays=0;

	 //Calculate the leap years before the year y
	 leapyears=(y-1)/4-(y-1)/100+(y-1)/400;

	 //check if y is leap year
	 if(y%400==0||y%100!=0&&y%4==0)
	    days[1]=29;
	 else
	    days[1]=28;

	totaldays=leapyears+(y-1)*365L;

	//Calculate days before month m in year y
	for(j=0;j<=m;j++)
	   thisyrdays=thisyrdays+days[i];

	//Calculate number of days that could not be evened out in weeks
	firstday=(int)((totaldays+thisyrdays)%7);

	clrwin(5,5,20,70,17);

	for(i=5;i<=70;i++)
	     writechar(' ',5,i,72);
	writestring("CALENDER",5,32,23);

	writestring("A",8,55,25);
	writestring("T S R",12,53,25);
	writestring("C A L E N D E R",17,49,25);

	writestring(months[m-1],6,9,23);

	itoa(y,temp,10);
	writestring(temp,6,29,23);

	writestring("Mon   Tue   Wed   Thu   Fri   Sat   Sun",8,7,23);

	//calculate column for first day of month

	k=7+firstday*6;
	i=10;

	//display Calender

	for(x=1;x<=days[m-1];x++)

	   {

	      itoa(x,temp1,10);
	      writestring(temp1,i,k,23);
	      k+=6;

	      //if September 1752 knock off 11 days to accomodate
	      //the change over Julian to Gregorian Calender

	      if(y==1752&&m==9&&x==2)
		 x=13;

	      if(k>43)

		{

		  i+=2;
		  k=7;

		}

	      if(i>18&&k==7)
		 i=10;

	   }

	  getkey();
	  writestring("Arrow Key To Change[ESC to Abort]",21,7,23);

	  if(ascii==0)

	    {

	    switch(scan)

		{

		   case RIGHT:

			if(m==12)

			  {

			     y=y+1;
			     m=1;

			  }

			 else
			     m=m+1;
			 break;

		   case LEFT:

			if(m==1)

			  {

			    y=y-1;
			    m=12;

			  }
			else
			    m=m-1;
			break;

		   case UP:

			y++;
			break;

		   case DOWN:

			y--;
			break;

	       }

	     }
	    }while(ascii!=ESC);
	    displaybuffer();
      }


 //process carriage return

void enter()

   {

      //if insert is on

      if(insert==1)

       {

	//if last line of file is last line of window

	if(er==23)

	  {

	    display_error("Buffer full, Can't continue...");
	    return;

	  }

	//move characters from current position onward in the buffer
	//forward by one byte

//------------------------------------------------------------------------

//HELP memmove()

//------------------------------------------------------------------------

// Copies a block of n bytes from src to dest

//-----------------------------------------------------------------------

	memmove(notepad_buffer+fpos+1,notepad_buffer+fpos,fsize-fpos);

	//insert carriage return
	notepad_buffer[fpos++]='\n';

	//increment fsize

	fsize++;

	//beginning of line pointer is current position

	line_beg=fpos;

	//current row and column positions are begining of the next line

	nc=1;
	nr++;

	//increment ending row

	er++;

	//display editor window

	displaybuffer();

     }

     else  //if insert is OFF

       {

	  if(nr==er)

	    {
	      //if current position is the end of file and ending row is
	      //less than the maximum number of rows allowed

	      if(fpos==fsize&&er<23)

		{

		  notepad_buffer[fpos]='\n';
		  fpos++;
		  fsize++;
		  line_beg=fpos;
		  nc=1;
		  nr++;
		  er++;

		}

	    }

	else    //if current line is not last line

	  {

	    while(notepad_buffer[fpos]!='\n')
	      fpos++;
	    fpos++;

	    line_beg=fpos;
	    nr++;
	    nc=1;

	 }

     }

   }

//process the left arrow key

void left()

  {

    //do nothing if already at the beginning of line

    if(nc==1)
       return;

    //decrement current position and column pointer

    fpos--;
    nc--;

 }

 //process right arrow key

void right()

  {

    //do nothing if already at end of line

    if(nc==78||notepad_buffer[fpos]=='\n'||fpos==fsize)
      return;

    fpos++;
    nc++;

  }

//process up arrow key

void up()

  {

    int ut,ll;

    //do nothing if already at top position
    if(nr==1)
      return;

    if(notepad_buffer[fpos]=='\n')
      fpos--;

    while(notepad_buffer[fpos]!='\n')
      fpos--;

    ut=fpos-1;
    while(notepad_buffer[ut]!='\n'&&ut>0)
      ut--;
    if(ut>0)
      ut++;

    line_beg=ut;
    if((ll=fpos-ut)<nc)
       nc=ll+1;
    else
      fpos=ut+nc-1;
    nr--;

  }

//process down arrow key

void down()

  {

    int ut,ll;

    //do nothing if already at the end line
    if(nr==er)
      return;

    while(notepad_buffer[fpos]!='\n')
	fpos++;
    fpos++;

    ut=fpos;
    ll=1;

    while(notepad_buffer[fpos]!='\n'&&fpos<fsize&&ll<nc)

      {

	fpos++;
	ll++;

      }

      if(ll<nc)
	 nc=ll;

     line_beg=ut;
     nr++;

  }

//process a backspace key

void backspace()

  {

    int lb;

    //do nothing if already at begining of file
    if(nc==1&&nr==1)
      return;

    memmove(notepad_buffer+fpos-1,notepad_buffer+fpos,fsize-fpos+1);
    fpos--;
    fsize--;

    //if \b is hit at the beginning of the line

    if(nc==1)

      {

	 //remove the \n of the previous line

	 lb=fpos;
	 if(notepad_buffer[lb]=='\n')
	   lb--;

	 while(notepad_buffer[lb]!='\n'&&lb>0)
	    lb--;
	 if(lb>0)
	    lb++;

	 nc=1+(fpos-lb);

	 nr--;
	 line_beg=lb;

	 if(lsize(line_beg)>=77)

	   {

	     memmove(notepad_buffer+line_beg+78,notepad_buffer+line_beg+77,
		     fsize-(line_beg+76));
	     notepad_buffer[line_beg+77]='\n';
	     display_error("Line too long,CR inserted.");

	   }

	else

	   er--;
	displaybuffer();

      }

      else

       {

	 nc--;
	 if(notepad_buffer[fpos]=='\n'||fpos==fsize)
	    writechar(' ',nr,nc,112);
	 else
	    displayline(line_beg,nr);

      }

  }


//process a del key

void del()

  {

    char ent;

    //do nothing if at end of line

    if(fpos==fsize)
       return;
    ent=notepad_buffer[fpos];

    memmove(notepad_buffer+fpos,notepad_buffer+fpos+1,fsize-fpos);

    fsize--;

    if(lsize(line_beg)>=77)

      {

	//insert a carriage return

	memmove(notepad_buffer+line_beg+78,notepad_buffer+line_beg+77,
	       fsize-(line_beg+76));
	       notepad_buffer[line_beg+77]='\n';
	       display_error("Line too long,CR inserted.");

     }

    //if del was hit at end of line

    if(ent=='\n')

      {

	displaybuffer();
	er--;

      }

    else

      displayline(line_beg,nr);

 }

//save current file

void save()

  {

    if(strcmpi(filename,"NONAME00.TMP")==0)
       getname(tempfile,67);
    else
       strcpy(tempfile,filename);

   handle=open(tempfile,O_CREAT|O_WRONLY|O_TEXT);
   if(handle==-1)

      {

	display_error("Can't create file");
	displaybuffer();
	return;

      }

   write(handle,notepad_buffer,fsize);
   close(handle);

   strcpy(filename,tempfile);

   for(j=0;j<80;j++)
      writechar(' ',0,j,71);

   writestring(filename,0,1,71);
   displaybuffer();

 }

//load a file from disk

void load()

  {

    int lchk;

    getname(tempfile,67);

    handle=open(tempfile,O_TEXT|O_RDWR|O_TEXT);

    if(handle==-1)

       {

	 display_error("Can't load file");
	 displaybuffer();
	 return;

      }

    fsize=read(handle,notepad_buffer,MAX);
    close(handle);

    strcpy(filename,tempfile);

    for(j=0;j<80;j++)

      {

	writechar(' ',0,j,71);

      }

    writestring(filename,0,1,71);
    displaybuffer();
    er=nr=nc=1;

    for(lchk=0;lchk<fsize;lchk++)
      if(notepad_buffer[lchk]=='\n')
	er++;

    line_beg=fpos=0;


  }

//start a new file

void new1()

  {

    strcpy(filename,"NONAME00.TMP");
    for(j=0;j<80;j++)
      writechar(' ',0,j,71);

    writestring(filename,0,1,71);

    line_beg=fsize=fpos=0;
    displaybuffer();
    er=nr=nc=1;

 }

//asks the filename

void getname(char fn[],int size)

  {

    clrwin(8,15,12,67,71);
    writestring("Enter filename",9,17,7);

    for(j=0;j<50;j++)
      writechar(' ',10,17+j,112);

   getstring(fn,size,11,18);

 }

//display the entire buffer on the edit screen

void displaybuffer()

  {

    int l;
    clrwin(1,1,23,78,112);

    for(l=1,k=0;l<=er;l++)

      {

	displayline(k,l);
	while(notepad_buffer[k]!='\n')
	   k++;
	k++;

      }

  }
