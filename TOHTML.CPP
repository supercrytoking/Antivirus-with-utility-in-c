 /***********************\T O H T M L . C P P *******************************
/ ToHTML is a program that converts C/C++/Header files into HTML Format
/ Using Gray for comments.
/ Version : 1.1
/ Author  : Manish
/ Email   : manishsnehi@rediffmail.com
/ Feel free to Email me about anything
/ I'd be glad to have a chat with you if u'd like :)
/***************************************************************************/

// I N C L U D E S //
#include <stdio.h>
#include <iostream.h>
#include <string.h>
//#include <ftimer.cpp>
#include <conio.h>
#include <stdlib.h>


// END OF INCLUDES //


// F U N C T I O N  	P R O T O T Y P E S //

// Takes a C/C++/Header line and converts it to a simple HTML line
// and outputs it to the file pointed to By the FILE pointer
// Called By Function : Convert()
// Parameters : char* Line
void ProcessLine(char*,FILE*);

// This is the main function , converts a C/C++/Header file to
// a simple HTML document.
// Called by Function : main()
// Parameters : char* SrcFile , char* DestFile
void Convert(char*,char*);


// END OF FUNCTION PROTOTYPES //

// H T M L   D E F I N E S //

// Document Related stuff //
#define LINK(x,y) "<SCRIPT language="JavaScript">\n\
			 <!--Hide\n \
			  MakeLink("##x","##y");\n\
			  //-->\n\
			  </SCRIPT>\n"

#define HTML_START "<HTML>\n<HEAD>\n<TITLE></TITLE>\n<SCRIPT TYPE="text/javascript" SRC="common.js"></SCRIPT>\n</HEAD>\n"
#define BODY_START "<BODY TEXT="FFFFFF" VLINK="FF0000" ALINK="FF0000" BGCOLOR="000000">\n"
#define BODY_END "</BODY>\n"
#define HTML_END "</HTML>\n"

// Font Related stuff//
#define BASEFONT "<BASEFONT FACE="Footlight MT Light" COLOR="FFFFFF" SIZE="3">\n"
#define COMMENT_FONT "<FONT COLOR="ADADAD">\n"
#define GREEN_FONT "<FONT COLOR="00FF00">\n"
#define FONT_END "</FONT>"


#define LINE_LENGTH 160
// END OF HTML DEFINES


// F U N C T I O N :   MAIN
void main()
{
 int Choice;
 char *SrcFile,*DestFile;
 LOOP:
 Choice = -200;
 while(Choice == -200)
 {
  clrscr();
  cout<<"Convert : "<<endl<<endl<<endl
	<<"          (1) C      file(*.c)"<<endl
	<<"          (2) Cpp    file(*.cpp)"<<endl
	<<"          (3) Header file(*.h)"<<endl;
  Choice = getch();
  switch(Choice)
  {
   case '1' : clrscr();
		  system("dir *.c /w");
		  break;

   case '2' : clrscr();
		  system("dir *.cpp /w");
		  break;

   case '3' : clrscr();
		  system("dir *.h  /w");
		  break;
   default  : goto LOOP;
  }//switch end
 }//while end
 cout<<"Enter Name of Source File(without extension)"<<endl;
 gets(SrcFile);
 cout<<"Enter Name of Destination File(without extension)"<<endl;
 gets(DestFile);
 switch(Choice)
 {
  case '1' : strcat(SrcFile,".c");
		 break;

  case '2' : strcat(SrcFile,".cpp");
		 break;

  case '3' : strcat(SrcFile,".h");
		 break;
 }
 strcat(DestFile,".htm");
 // Call the converting function
 Convert(SrcFile,DestFile);
 // clear screen after conversion
 clrscr();
 cout<<"Would you like to convert another file : "<<endl
     <<"(y)es or (n)o ";
 Choice = getch();
 if(Choice == 'y') goto LOOP;
 else exit(0);
}


// F U N C T I O N :   CONVERT
void Convert(char* SrcFile,char* DestFile)
{
// TIMER tClock;
 int dump;
 int x,y;
 char *Line;
 FILE *Fin,*Fout;
 // Open Both files
 if((Fin = fopen(SrcFile,"rt")) == NULL)
 {
  cerr<<"Unable to open file "<<SrcFile<<" for text reading";
  dump = getch();
  exit(0);
 }
 if((Fout = fopen(DestFile,"wt")) == NULL)
 {
  cerr<<"Unable to open file "<<DestFile<<" for text writing";
  dump = getch();
  exit(0);
 }
 // Both files opened now.
 cout<<"File : "<<SrcFile <<" opened for reading"<<endl
     <<"File : "<<DestFile<<" opened for writing"<<endl;
 // Put The Basic Stuff in DestFile
 //tClock.Start();
 fprintf(Fout,HTML_START);
 fprintf(Fout,BODY_START);
 fprintf(Fout,BASEFONT);
 cout<<"Generated Head and Body Start"<<endl;
 int LineNum=1;
 cout<<"Processing line Number ";
 x = wherex();
 y = wherey();
 while((fgets(Line,LINE_LENGTH,Fin)) != NULL)
 {
  ProcessLine(Line,Fout);
  gotoxy(x,y);
  cout<<LineNum++<<".....complete"<<endl;
  clreol();
 }
 // Print the last line
 fprintf(Fout,"\n%s",Line);
 // Print End Stuff
 fprintf(Fout,BODY_END);
 fprintf(Fout,HTML_END);
// cout<<"Ending Body And Head and Document...Complete"<<endl
//     <<"Time Elapsed : "<<tClock.Elapsed()<<" seconds"<<endl;
 // Close the files
 fclose(Fin);
 fclose(Fout);
 cout<<"Files Closed"<<endl;
 dump = getch();
 dump++;
}

void ProcessLine(char* Line,FILE* Fout)
{
 int length = strlen(Line)+1;
 int index = 0;
 int Comment = 0;
 while(index < length )
 {
  switch(Line[index])
  {
   case '#'  : if(Comment != 1)
		   {
		    fprintf(Fout,GREEN_FONT);
		    Comment = 1;
		   }
		   fprintf(Fout,"#");
		   index++;
		   break;

   case '\n' : fprintf(Fout,"<BR>\n");
		   index++;
		   break;

   case ' '  : fprintf(Fout," ");
		   index++;
		   // If consecutive spaces make with &nbsp;
		   while(Line[index] == ' ')
		   {
		    fprintf(Fout,"&nbsp;");
		    index++;
		   }
		   break;

   case '<'  : fprintf(Fout,"&lt;");
		   index++;
		   break;

   case '>'  : fprintf(Fout,"&gt;");
		   index++;
		   break;

   case '/'  :
		   // If a single line Comment: // found Copy Font Comment Tag in ReturnedLine and make Comment = 1;
		   if(Line[index+1] == '/' && Comment!=1)
		   {
		    fprintf(Fout,COMMENT_FONT);
		    Comment = 1;
		    index+=2;
		    fprintf(Fout,"//");
		   }
		   // If Comment : /* found Copy Font Comment Tag only
		   else if(Line[index+1] == '*' && Comment!=1)
		   {
		    fprintf(Fout,COMMENT_FONT);
		    index+=2;
		    fprintf(Fout,"/*");
		   }
		   else
		   {
		    fprintf(Fout,"/");
		    index++;
		   }
		   break;

   case '*'  : //Ending a comment with : */
		   if(Line[index+1] == '/')
		   {
		    Comment = 1;
		    fprintf(Fout,"*/");
		    index+=2;
		   }
		   else
		   {
		    fprintf(Fout,"*");
		    index++;
		   }
		   break;

   default   : fprintf(Fout,"%c",Line[index]);
		   index++;
		   break;
  }//switch end
 }//while end
 if(Comment == 1) fprintf(Fout,FONT_END);
} 