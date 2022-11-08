 
/* Program to create backup of the AUTOEXEC.BAT file */

#include <stdio.h>
#include <conio.h>
#include<dos.h>


int main(void)
{
   FILE *in, *out,*final;
   int i = 0;
   char ch;
   char p;
   char str1[]="c:\\windows\\command\\clock.exe\nc:\\windows\\command\\notepad.exe\n";

   clrscr();

   if ((in = fopen("C:\\autoexec.bat", "rt"))
       == NULL)
   {
      fprintf(stderr, "Cannot open input file.\n");
      return 1;
   }

   if ((out = fopen("C:\\TEMP.BAT", "wt"))
       == NULL)
   {
      fprintf(stderr, "Cannot open output file.\n");
      return 1;
   }

   while (!feof(in))
      {
	p=fgetc(in);
	if(p==EOF)
	   break;
	fputc(p, out);
      }

   while (str1[i])
      {
	 fputc(str1[i],out);
	 i++;
      }
   fclose(out);
   fclose(in);
   return 0;
}