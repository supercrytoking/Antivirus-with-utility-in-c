 # include "iostream.h"
# include "stdio.h"
# include "process.h"
# include "string.h"
# include "stdlib.h"
# include "ctype.h"
# include "conio.h"
# include "fcntl.h"
# include "types.h"
# include "stat.h"
# include "dir.h"
# include "dos.h"
# include "graphics.h"

# include "d:\manish\softwa~1\utility\utility\intro.h"


// Class to provide the file services
class  file_class
{
	public:

		// Function to delete a file
		delete_file(char far *memory);

		// Function to display the current attribute of files
		get_attb(char far *memory) ;

		// Sets new attributes to a file
		set_attb(char far *memory);


		file_attb(char far *memory) ;
		encrypt_file(char far *memory) ;
		decrypt_file(char far *memory) ;
		compress_file(char far *memory) ;
		decompress_file(char far *memory) ;
		type_file(char far *memory) ;
		writeattr ( int attb, int col ,char far *memory);
};

// Delete the desired file
file_class :: delete_file(char far *memory)
{
	union REGS iregs, oregs ;
	int area ;
	char fname[20], *m ;

	area = ( 20 - 5 + 1 ) * ( 70 - 5 + 1 ) * 2 ;
	m = ((char *) malloc ( area )) ;
	if ( m == NULL )
	{
		writeline ( "           Press any key to continue...             ", 22, 14, 12,memory ) ;
		getch() ;

		writeline ("          Select using hotkey or Enter              ", 22, 14, 12,memory ) ;
		return 0;
	}

	store_video ( 5, 5, 20, 70, m ,memory) ;
	menubox ( 8, 5, 13, 60, 277, 1,memory ) ;

	writeline ( "                File delete service                 " , 22, 14, 112,memory ) ;

	writeline ( "Enter name of file to be deleted:", 9, 8, 12,memory ) ;
	size ( 6, 7 ) ;
	gotoxy ( 43, 10 ) ;
	gets ( fname ) ;
	size ( 32, 0 ) ;

	/* issue interrupt for deleting file */
	iregs.h.ah = 65 ;  /* dos service number */
	iregs.x.dx = ( unsigned int ) fname ;  /* store base address */
	intdos ( &iregs, &oregs ) ;

	/* check if successful in deleting file */
	if ( oregs.x.cflag == 0 )
		writeline ( "File was successfully deleted!", 11, 8, 12,memory ) ;
	else
	{
		switch ( oregs.x.ax )
		{
				case 2 :
				writeline ( "File not found!", 11, 8, 12,memory ) ;
				break ;

			case 3 :
				writeline ( "Invalid path!", 11, 8, 12,memory ) ;
				break ;

			case 5 :
				writeline ( "Access denied!", 11, 8, 12,memory ) ;
				break ;

			case 0x11 :
				writeline ( "Invalid drive name!",11, 8,12 ,memory) ;
				break ;

			default :
				writeline ( "Improper request!", 11, 8, 12 ,memory) ;
		}
	}

	writeline ( "             Press Enter to return                   ", 22, 14, 12,memory ) ;
	getch() ;
	writeline ( "          Select using hotkey or Enter              ", 22, 14, 12 ,memory) ;
	restore_video ( 5, 5, 20, 70, m,memory ) ;
	free ( m ) ;
}

/* displays the current attributes of a file */
file_class::get_attb(char far *memory)
{
	union REGS iregs, oregs ;
	int a, area ;
	char fname[20], *m ;

	area = ( 20 - 5 + 1 ) * ( 70 - 5 + 1 ) * 2 ;
	m =((char *) malloc ( area ) );
	if ( m == NULL )
	{
		writeline ( "           Press any key to continue...             ", 22, 14, 12,memory ) ;
		getch() ;

		writeline ("          Select using hotkey or Enter              ", 22, 14, 12,memory ) ;
		return( 0);
	}

	store_video ( 5, 5, 20, 70, m,memory ) ;
	menubox ( 8, 5, 18, 60, 227, 1,memory ) ;

	writeline ( "             Get file attribute service               ", 22, 14, 12 ,memory) ;

	writeline ( "Enter name of file:", 9, 8, 12 ,memory) ;
	size ( 6, 7 ) ;
	gotoxy ( 29, 10 ) ;
	gets ( fname ) ;
	size ( 32, 0 ) ;

	iregs.h.ah = 67 ;  /* dos service number */
	iregs.h.al = 0 ;  /* 0 - get attributes, 1 - set attributes */
	iregs.x.dx = ( unsigned int ) fname ;  /* store base address */
	intdos ( &iregs, &oregs ) ;  /* issue interrupt */

	/* if successful display attributes, else display error message */
	if ( oregs.x.cflag == 0 )
	{
		writeline ( "ATTRIBUTES", 11, 27, 12 ,memory) ;
		writeline ( "ÄÄÄÄÄÄÄÄÄÄ", 12, 27, 12 ,memory) ;
		a = oregs.x.cx ;
		writeattr ( a, 24,memory ) ;
	}
	else
	{
		switch ( oregs.x.ax )
		{
			case 2 :
				writeline ( "File not found!", 11, 8, 12 ,memory) ;
				break ;

			case 3 :
				writeline ( "Invalid path!", 11, 8, 12,memory ) ;
				break ;

			case 5 :
				writeline ( "Access denied!", 11, 8, 12,memory ) ;
				break ;

			case 0x11 :
				writeline ( "Invalid drive name!", 11, 8, 12,memory ) ;
				break ;

			default :
				writeline ( "Improper request!", 11, 8, 12,memory ) ;
		}
	}

	writeline ( "             Press Enter to return                   ", 22, 14, 12,memory ) ;
	getch() ;

	writeline ("          Select using hotkey or Enter              ", 22, 14, 12,memory ) ;
	restore_video ( 5, 5, 20, 70, m,memory ) ;
	free ( m ) ;
}

/* displays attributes of the file */
file_class::writeattr ( int attb, int col,char far *memory )
{
	writeline ( "Read only :", 13, col, 12 ,memory) ;
	if ( ( attb & 1 ) == 0 )
		writeline ( "OFF", 13, ( col + 12 ), 12,memory ) ;
	else
		writeline ( "ON", 13, ( col + 12 ), 12 ,memory) ;

	writeline ( "Hidden    :", 14, col, 12,memory ) ;
	if ( ( attb & 2 ) == 0 )
		writeline ( "OFF", 14, ( col + 12 ), 12,memory ) ;
	else
		writeline ( "ON", 14, ( col + 12 ), 12 ,memory) ;

	writeline ( "System    :", 15, col, 12 ,memory) ;
	if ( ( attb & 4 ) == 0 )
		writeline ( "OFF", 15, ( col + 12 ), 12,memory ) ;
	else
		writeline ( "ON", 15, ( col + 12 ), 12,memory ) ;

	writeline ( "Archive   :", 16, col, 12,memory ) ;
	if ( ( attb & 32 ) == 0 )
		writeline ( "OFF", 16, ( col + 12 ),12,memory ) ;
	else
		writeline ( "ON", 16, ( col + 12 ), 12 ,memory) ;
}

/* sets new attributes for a file */
file_class::set_attb(char far *memory)
{
	union REGS iregs, oregs ;
	int old, new1, area ;
	char fname[20], *m, ch ;

	area = ( 20 - 5 + 1 ) * ( 70 - 5 + 1 ) * 2 ;
	m = ((char *) malloc ( area )) ;
	if ( m == NULL )
	{
		writeline ("       press any key to continue    ", 22, 14, 12,memory ) ;
		getch() ;

		writeline ( "          Select using hotkey or Enter              ", 22, 14, 12,memory ) ;
		return 0;
	}

	store_video ( 5, 5, 20, 70, m ,memory) ;
	menubox ( 8, 5, 18, 60, 277, 1,memory ) ;

	writeline ( "             Set file attribute service               ", 22, 14, 12 ,memory) ;

	writeline ( "Enter name of file:", 9, 8, 12,memory ) ;
	size ( 6, 7 ) ;
	gotoxy ( 29, 10 ) ;
	gets ( fname ) ;
	size ( 32, 0 ) ;

	iregs.h.ah = 67 ;  /* dos service number */
	iregs.h.al = 0 ;  /* 0 - get attributes, 1 - set attributes */
	iregs.x.dx = ( unsigned int ) fname ;  /* base address of filename */
	intdos ( &iregs, &oregs ) ;  /* issue interrupt */

	/* if successful display attributes, else display error message */
	if ( oregs.x.cflag == 0 )
	{
		old = new1 = oregs.x.cx ;
		writeline ( "Existing Attributes",11, 8, 12,memory ) ;
		writeline ( "-------------------", 12, 8, 12,memory ) ;
	      writeattr ( old, 8 ,memory) ;  /* display existing attributes */
	}
	else
	{
		switch ( oregs.x.ax )
		{
			case 2 :
				writeline ( "File not found!", 11, 8,12 ,memory) ;
				break ;

			case 3 :
				writeline ( "Invalid path!", 11, 8, 12,memory ) ;
				break ;

			case 5 :
				writeline ( "Access denied!", 11, 8, 12,memory ) ;
				break ;

			case 0x11 :
				writeline ( "Invalid drive name!",11, 8, 12 ,memory) ;
				break ;

			default :
				writeline ( "Improper request!", 11, 8, 12,memory ) ;
		}

		writeline (  "             Press Enter to return                   ", 22, 14, 12,memory ) ;
		getch() ;

		restore_video ( 5, 5, 20, 70, m,memory) ;
		free ( m ) ;
		return 0;
	}

	/* collect new attributes or keep old attributes */
	writeline ( "Change (Y/N):", 13, 30, 12,memory ) ;
	size ( 6, 7 ) ;
	gotoxy ( 45, 14 ) ;
	fflush ( stdin ) ;  /* flush keyboard buffer */
	ch = getch() ;
	ch = toupper ( ch ) ;
	writech ( 13, 44, ch, 112,memory ) ;

	if ( ch == 'Y' )
		new1 = ( new1 ^ 1 ) ;

	writeline ( "Change (Y/N):", 14, 30, 12,memory ) ;
	gotoxy ( 45, 15 ) ;
	fflush ( stdin ) ;
	ch = toupper ( getch() ) ;
	writech ( 14, 44, ch, 112 ,memory) ;

	if ( ch == 'Y' )
		new1 = ( new1 ^ 2 ) ;

	writeline ( "Change (Y/N):", 15, 30, 12,memory ) ;
	gotoxy ( 45, 16 ) ;
	fflush ( stdin ) ;
	ch = toupper ( getch() ) ;
	writech ( 15, 44, ch, 112,memory ) ;

	if ( ch == 'Y' )
		new1 = ( new1 ^ 4 ) ;

	writeline ( "Change (Y/N):", 16, 30, 12,memory ) ;
	gotoxy ( 45, 17 ) ;
	fflush ( stdin ) ;
	ch = toupper ( getch() ) ;
	writech ( 16, 44, ch, 112,memory ) ;

	size ( 32, 0 ) ;

	if ( ch == 'Y' )
		new1 = ( new1 ^ 32 ) ;

	/* issue interrupt to set new file attributes */
	iregs.h.ah = 67 ;
	iregs.h.al = 1 ;
	iregs.x.cx = new1 ;
	iregs.x.dx = ( unsigned int ) fname ;
	intdos ( &iregs, &oregs ) ;

	menubox ( 8, 5, 18, 60, 12, 66,memory ) ;
	writeline ( "File name:", 9, 8, 12,memory ) ;
	writeline ( fname, 10, 19, 12 ,memory) ;

	/* if successful display old and new attributes, else display error message */
	if ( oregs.x.cflag == 0 )
	{
		writeline ( "Old Attributes", 11, 8, 12,memory ) ;
		writeline ( "--------------", 12, 8, 12,memory ) ;
		writeattr ( old, 8 ,memory) ;
		writeline ( "New Attributes", 11, 40, 12,memory ) ;
		writeline ( "--------------", 12, 40, 12 ,memory) ;
		writeattr ( new1, 40 ,memory) ;
	}
	else
		writeline ( "Error - New attributes not set!", 13, 8, 12,memory ) ;

	writeline ( "             Press Enter to return                   ", 22, 14, 12 ,memory) ;
	getch() ;

	writeline ( "          Select using hotkey or Enter              ", 22, 14, 12,memory ) ;
	restore_video ( 5, 5, 20, 70, m,memory ) ;
	free ( m ) ;
}

// Encrypt the desired file
file_class::encrypt_file(char far *memory)
{
	char s_file[20], t_file[20], *m, ch ;
	FILE *fp1 , *fp2 ;
	int area, flag ;

	area = ( 20 - 5 + 1 ) * ( 70 - 5 + 1 ) * 2 ;
	m =((char *) malloc ( area )) ;
	if ( m == NULL )
	  {
	      writeline("   Press any key to continue...",22,14,12,memory);
	      getch();

	      writeline("   Select using hot key or Enter...",22,14,12,memory);
	      return 0;
	   }

	writeline ( "                File coding service                  ", 22, 14, 12 ,memory) ;

	writeline ( "Enter source file name:", 7, 8, 12,memory ) ;
	size ( 6, 7 ) ;
	gotoxy ( 33, 8 ) ;
	fflush ( stdin ) ;
	gets ( s_file ) ;
	size ( 32, 0 ) ;

	/* open source file */
	fp1 = fopen ( s_file, "r" ) ;
	if ( fp1 == NULL )
	{
		writeline ( "Unable to open source file!", 9, 8, 12,memory ) ;
		writeline (  "             Press Enter to return                   ", 22, 14, 12,memory ) ;
		getch() ;

		writeline ( "          Select using hotkey or Enter              ", 22, 14, 12,memory ) ;
		restore_video ( 5, 5, 20, 70, m ,memory) ;
		free ( m ) ;
		return 0;
	}

	writeline ( "Enter target file name:", 9, 8, 12,memory ) ;
	size ( 6, 7 ) ;
	gotoxy ( 33, 10 ) ;
	gets ( t_file ) ;
	size ( 32, 0 ) ;

	/* open target file */
	fp2 = fopen ( t_file, "w" ) ;
	if ( fp2 == NULL )
	{
		writeline ( "Unable to open target file!", 11, 8, 12,memory ) ;
		writeline ( "             Press Enter to return                   ", 22, 14, 12,memory ) ;
		getch() ;

		fclose ( fp1 ) ;
		writeline ( "          Select using hotkey or Enter              ", 22, 14, 12,memory ) ;
		restore_video ( 5, 5, 20, 70, m ,memory) ;
		free ( m ) ;
		return 0;
	}

	/* read each character, offset it by 127, write to target file */
	while ( ( ch = getc ( fp1 ) ) != EOF )
	{
		if ( ch == '\n' )
			flag = putc ( '\n', fp2 ) ;
		else
			flag = putc ( ( ch + 127 ), fp2 ) ;

		/* if error in writing */
		if ( flag == EOF )
			break ;
	}

	if ( flag == EOF )
		writeline ( "Unable to encrypt file!", 11, 8, 12,memory ) ;
	else
		writeline ( "File is successfully encrypted!", 11, 8, 12,memory ) ;

	writeline( "             Press Enter to return                   ", 22, 14, 12,memory ) ;
	getch() ;

	fclose ( fp1 ) ;
	fclose ( fp2 ) ;
	writeline ("          Select using hotkey or Enter              ", 22, 14, 12,memory ) ;
	restore_video ( 5, 5, 20, 70, m ,memory) ;
	free ( m ) ;
}

// Decrypt the desired file
file_class::decrypt_file(char far *memory)
{
	char s_file[20], t_file[20], *m, ch ;
	FILE *fp1, *fp2 ;
	int area, flag ;

	area = ( 20 - 5 + 1 ) * ( 70 - 5 + 1 ) * 2 ;
	m =((char *) malloc ( area ) );
	if ( m == NULL )
	{
		writeline ( "             Press any key to continue        ", 22, 14, 12,memory ) ;
		getch() ;

		writeline ( "          Select using hotkey or Enter              ", 22, 14, 12,memory ) ;
		return 0;
	}

	store_video ( 5, 5, 20, 70, m,memory ) ;
	menubox ( 6, 5, 13, 60, 227, 1 ,memory) ;

	writeline ( "                File decode service                 ", 22, 14, 112,memory ) ;

	writeline ( "Enter source file name:", 7, 8, 12,memory ) ;
	size ( 6, 7 ) ;
	gotoxy ( 33, 8 ) ;
	gets ( s_file ) ;
	size ( 32, 0 ) ;

	/* open source file */
	fp1 = fopen ( s_file, "r" ) ;
	if ( fp1 == NULL )
	{
		writeline ( "Unable to open source file!", 9, 8, 12,memory ) ;
		writeline (  "             Press Enter to return                   ", 22, 14, 12 ,memory) ;
		getch() ;

		writeline ( "          Select using hotkey or Enter              ", 22, 14, 12 ,memory) ;
		restore_video ( 5, 5, 20, 70, m ,memory) ;
		free ( m ) ;
		return 0;
	}

	writeline ( "Enter target file name:", 9, 8, 12,memory ) ;
	size ( 6, 7 ) ;
	gotoxy ( 33, 10 ) ;
	gets ( t_file ) ;
	size ( 32, 0 ) ;

	/* open target file */
	fp2 = fopen ( t_file, "w" ) ;
	if ( fp2 == NULL )
	{
		writeline ( "Unable to open target file!", 11, 8, 12,memory ) ;
		writeline ( "             Press Enter to return                   ", 22, 14, 12,memory ) ;
		getch() ;

		fclose ( fp1 ) ;
		writeline ( "          Select using hotkey or Enter              ", 22, 14, 12 ,memory) ;
		restore_video ( 5, 5, 20, 70, m ,memory) ;
		free ( m ) ;
		return 0;
	}

	/* read each character, reduce by offset, write to target file */
	while ( ( ch = getc ( fp1 ) ) != EOF )
	{
		if ( ch == '\n' )
			flag = putc ( '\n', fp2 ) ;
		else
			flag = putc ( ( ch - 127 ), fp2 ) ;

		/* if error in writing */
		if ( flag == EOF )
			break ;
	}

	if ( flag == EOF )
		writeline ( "Unable to decrypt file!", 11, 8, 12,memory ) ;
	else
		writeline ( "File successfully decrypted!", 11, 8, 12,memory ) ;

	writeline (  "             Press Enter to return                   ", 22, 14, 12 ,memory) ;
	getch() ;

	fclose ( fp1 ) ;
	fclose ( fp2 ) ;
	writeline ( "          Select using hotkey or Enter              ", 22, 14, 12 ,memory) ;
	restore_video ( 5, 5, 20, 70, m ,memory) ;
	free ( m ) ;
}

// Compress the desired file
file_class::compress_file(char far *memory)
{
	char *m, s_file[20], t_file[20] ;
	FILE *fp1, *fp2 ;
	int area, count, ch, flag ;

	area = ( 12 - 5 + 1 ) * ( 60 - 5 + 1 ) * 2 ;
	m =((char *) malloc ( area )) ;
	if ( m == NULL )
	{
		writeline ( "       press any key to continue       ", 22, 14, 12,memory ) ;
		getch() ;

		writeline ("          Select using hotkey or Enter              ", 22, 14, 12 ,memory) ;
		return 0;
	}

	store_video ( 5, 5, 12, 60, m,memory ) ;
	menubox ( 5, 5, 12, 60, 227, 1 ,memory) ;

	writeline ( "               File compress service                ", 22, 14, 112 ,memory) ;

	writeline ( "Enter source file name:", 6, 8, 12 ,memory) ;
	size ( 6, 7 ) ;
	gotoxy ( 33, 7 ) ;
	gets ( s_file ) ;
	size ( 32, 0 ) ;

	/* open source file */
	fp1 = fopen ( s_file, "r" ) ;
	if ( fp1 == NULL )
	{
		writeline ( "Unable to open source file!", 8, 8, 12 ,memory) ;
		writeline (  "             Press Enter to return                   ", 22, 14, 12,memory ) ;
		getch() ;

		writeline ("          Select using hotkey or Enter              ", 22, 14, 12 ,memory) ;
		restore_video ( 5, 5, 12, 60, m ,memory) ;
		free ( m ) ;
		return 0;
	}

	writeline ( "Enter target file name:", 8, 8, 12,memory ) ;
	size ( 6, 7 ) ;
	gotoxy ( 33, 9 ) ;
	gets ( t_file ) ;
	size ( 32, 0 ) ;

	/* open target file */
	fp2 = fopen ( t_file, "w") ;
	if ( fp2 == NULL )
	{
		writeline ( "Unable to open target file!", 10, 8, 12,memory ) ;
		writeline ( "             Press Enter to return                   ", 22, 14, 12 ,memory) ;
		getch() ;

		fclose ( fp1 ) ;
		writeline ("          Select using hotkey or Enter              ", 22, 14, 12 ,memory) ;
		restore_video ( 5, 5, 12, 60, m ,memory) ;
		free ( m ) ;
		return 0;
	}

	/* read each character till end of file is reached */
	while ( ( ch = getc ( fp1 ) ) != EOF )
	{
		/* check for space */
		if ( ch == ' ' )
		{
			count = 1 ;

			/* count number of consecutive spaces */
			while ( ( ch = getc ( fp1 ) ) == ' ' )
				count++ ;

			flag = putc ( count + 127, fp2 ) ;
			flag = putc ( ch, fp2 ) ;
		}
		else
			flag = putc ( ch, fp2 ) ;

		if ( flag == EOF )
			break ;
	}

	if ( flag == EOF )
		writeline ( "Unable to compress file!", 10, 8,12,memory ) ;
	else
		writeline ( "File successfully compressed!", 10, 8, 12 ,memory) ;

	writeline (  "             Press Enter to return                   ", 22, 14, 12,memory ) ;
	getch() ;

	fclose ( fp1 ) ;
	fclose ( fp2 ) ;
	writeline ( "          Select using hotkey or Enter              ", 22, 14, 12 ,memory) ;
	restore_video ( 5, 5, 12, 60, m ,memory) ;
	free ( m ) ;
}


//Decompress the desired file
file_class::decompress_file(char far *memory)
{
	char *m, s_file[20], t_file[20] ;
	FILE *fp1, *fp2 ;
	int ch, count, area, flag ;

	area = ( 12 - 5 + 1 ) * ( 60 - 5 + 1 ) * 2 ;
	m =((char *) malloc ( area )) ;
	if ( m == NULL )
	{
		writeline ( "              Press any key to continue ", 22, 14, 12,memory ) ;
		getch() ;

		writeline ("          Select using hotkey or Enter              ", 22, 14, 12 ,memory) ;
		return 0;
	}

	store_video ( 5, 5, 12, 60, m,memory ) ;
	menubox ( 5, 5, 12, 60, 227, 1 ,memory) ;

	writeline ( "              File decompress service               ", 22, 14, 112 ,memory) ;
	writeline ( "Enter source file name:", 6, 8, 12 ,memory) ;
	size ( 6, 7 ) ;
	gotoxy ( 33, 7 ) ;
	gets ( s_file ) ;
	size ( 32, 0 ) ;

	/* open source file */
	fp1 = fopen ( s_file, "r" ) ;
	if ( fp1 == NULL )
	{
		writeline ( "Unable to open source file!", 8 , 8, 12 ,memory) ;
		writeline ( "             Press Enter to return                   ", 22, 14, 12 ,memory) ;
		getch() ;

		writeline ("          Select using hotkey or Enter              ", 22, 14, 12 ,memory) ;
		restore_video ( 5, 5, 12, 60, m ,memory) ;
		free ( m ) ;
		return 0;
	}

	writeline ( "Enter target file name:", 8, 8, 12,memory ) ;
	size ( 6, 7 );
	gotoxy ( 33, 9 ) ;
	gets ( t_file ) ;
	size ( 32, 0 ) ;

	/* open target file */
	fp2 = fopen ( t_file, "w" ) ;
	if ( fp2 == NULL )
	{
		writeline ( "Unable to open target file!", 10, 8, 12 ,memory) ;
		writeline ( "             Press Enter to return                   ", 22, 14, 12 ,memory) ;
		getch() ;

		fclose ( fp1 ) ;
		writeline ("          Select using hotkey or Enter              ", 22, 14, 12 ,memory) ;
		restore_video ( 5, 5, 12, 60, m ,memory) ;
		free ( m ) ;
		return 0;
	}

	/* read each character till end of file is reached */
	while ( ( ch = getc ( fp1 ) ) != EOF )
	{
		/* if ascii value of character read exceeds 127 */
		if ( ch > 127 )
		{
			ch = ch - 127 ;

			/* write back original spaces */
			for ( count = 1 ; count <= ch ; count++ )
				flag = putc ( ' ', fp2 ) ;
		}
		else
			flag = putc ( ch, fp2 ) ;

		/* if error in writing */
		if ( flag == EOF )
			break ;
	}

	if ( flag == EOF )
		writeline ( "Unable to decompress file!", 10, 8, 12 ,memory) ;
	else
		writeline ( "File successfully decompressed!", 10, 8, 12,memory ) ;

	writeline (  "             Press Enter to return                   ", 22, 14, 12 ,memory) ;
	getch() ;

	fclose ( fp1 ) ;
	fclose ( fp2 ) ;
	writeline ( "          Select using hotkey or Enter              ", 22, 14, 12 ,memory) ;
	restore_video ( 5, 5, 12, 60, m ,memory) ;
	free ( m ) ;
}

// Type the content of the file
file_class::type_file(char far *memory)
{
	char *m, fname[20], ch, string[5] ;
	int pg = 1, row = 2, col = 1, area ;
	FILE *fp ;

	area = ( 25 - 0 + 1 ) * ( 80 - 0 + 1 ) * 2 ;
	m =((char *) malloc ( area )) ;
	if ( m == NULL )
	{
		writeline ( "           Press any key to continue    ", 22, 14, 12,memory ) ;
		getch() ;

		writeline ( "          Select using hotkey or Enter              ", 22, 14, 12 ,memory) ;
		return 0;
	}

	store_video ( 0, 0, 24, 79, m ,memory) ;
	menubox ( 5, 5, 10, 60, 227, 1 ,memory) ;

	writeline ( "               File display service                 ", 22, 14, 112 ,memory) ;

	writeline ( "Enter name of file:", 6, 8, 12,memory ) ;
	size ( 6, 7 ) ;
	gotoxy ( 29, 7 ) ;
	gets ( fname ) ;
	size ( 32, 0 ) ;

	/* open file to be displayed */
	fp = fopen ( fname, "r" ) ;
	if ( fp == NULL )
	{
		writeline ( "Unable to open source file!", 8, 8,12 ,memory) ;
		writeline ( "             Press Enter to return                   ", 22, 14,12 ,memory) ;
		getch() ;

		writeline ( "          Select using hotkey or Enter              ", 22, 14, 12 ,memory) ;
		restore_video ( 0, 0, 24, 79, m ,memory) ;
		free ( m) ;
		return 0;
	}

	menubox ( 0, 0, 1, 79, 27, 27 ,memory) ;
	menubox ( 1, 0, 24, 79, 127, 32 ,memory) ;
	display_box ( 1, 0, 23, 78, 112,memory ) ;
	display_box ( 1, 0, 21, 78, 112,memory ) ;
	writech ( 21, 0, 204, 112 ,memory) ;
	writech ( 21, 78, 185, 112,memory ) ;

	/* display file name and current page number */
	writeline ( "File :-", 0, 0, 7,memory ) ;
	writeline ( fname, 0, 8, 7 ,memory) ;
	writeline ( "Page No :- ", 0, 54, 7,memory ) ;
	itoa ( pg, string, 10 ) ;
	writeline ( string, 0, 65, 7 ,memory) ;

	/* read each character till end of file is reached */
	while ( ( ch = getc ( fp ) ) != EOF )
	{
		/* if character read is not a newline, carriage return or tab */
		if ( ( ch != '\n' ) && ( ch != '\r' ) && ( ch != '\t' ) )
			writech ( row, col, ch, 112 ,memory) ;

		/* if tab, increment column by 4 otherwise by 1 */
		if ( ch == '\t' )
			col += 4 ;
		else
			col++ ;

		/* if column exceeds 77 or end of line is met */
		if ( col > 77 || ch == '\n' )
		{
			col = 1 ;
			row++ ;

			/* if screen is full */
			if ( row > 20 )
			{
				writeline ( "             Press Enter to return                   ", 22, 17, 12 ,memory) ;
				getch() ;

				row = 2 ;
				pg++ ;

				menubox ( 2, 1, 20, 77, 1, 0,memory ) ;

				writeline ( "File :-", 0, 0, 7 ,memory) ;
				writeline ( fname, 0, 8, 7,memory ) ;
				writeline ( "Page No :- ", 0, 54, 7,memory ) ;
				itoa ( pg, string, 10 ) ;
				writeline ( string, 0, 65, 7 ,memory) ;
			}
		}
	}

	writeline ( "            Press any key to return...               ", 22, 14, 12 ,memory) ;
	getch() ;

	fclose ( fp ) ;
	writeline ("          Select using hotkey or Enter              ", 22, 14, 12 ,memory) ;
	restore_video ( 0, 0, 24, 79, m ,memory) ;
	free ( m ) ;
}

// Class to provide the directory services
class  dir_class
{
	public:
	create_dir(char far *memory);
	list_dir(char far *memory);
	change_dir(char far *memory);
	delete_dir(char far *memory);
};

// Function to create the directory
dir_class::create_dir(char far *memory)
{
	union REGS iregs, oregs ;
	int area ;
	char dname[20], *m ;

	area = ( 20 - 5 + 1 ) * ( 70 - 5 + 1 ) * 2 ;
	m =((char *) malloc ( area) );
	if ( m == NULL )
	{
		writeline ("     Insufficient memory                     " , 22, 15, 112,memory ) ;
		getch();
		writeline("    Select using hotkey or Enter        ",22,14,12,memory);
		return 0;
	}
	store_video(5,5,20,70,m,memory);
	menubox(6,5,11,60,12,66,memory);
	writeline("          Create directory service             ",22,15,12,memory);
	writeline ( "Enter name of directory:", 7, 8, 112,memory ) ;
	size ( 6, 7 ) ;
	gotoxy ( 34, 8 ) ;
	gets ( dname ) ;
	size ( 32, 0 ) ;

	iregs.h.ah = 57 ;                   /*dos service number*/
	iregs.x.dx = (unsigned int) dname;       /* base address of directory name*/
	intdos(&iregs,&oregs);         /*  issue interrupt*/

	/*  if successful in creation of directory*/
	if (oregs.x.cflag == 0)
	writeline(" Directory created ",9,8,12,memory);
	else
	{
		if (oregs.x.ax == 5);
		writeline("Improper acces",9,8,12,memory);
		if ( oregs.x.ax == 3 )
			writeline ( "Invalid path!", 9, 8, 112,memory ) ;
	}

	writeline ("                  Press  any  key to continue               ", 22, 14, 112 ,memory) ;
	getch() ;

	writeline( "    Select using hotkey or Enter             ",22,14,112,memory);
	restore_video(5,5,20,70,m,memory);
	free(m);
}

// Function to change the directory
dir_class :: change_dir(char far *memory)
{
	union  REGS  iregs,oregs;
	int area ;
	char dname[20], *m ;

	area = ( 20 - 5 + 1 ) * ( 76 - 5 + 1 ) * 2 ;
	m =((char *) malloc ( area )) ;
	if ( m == NULL )
	{
		writeline ( "           Insufficient  memory                ", 22, 14, 12 ,memory) ;
		getch() ;

		writeline (  "          Select using hotkey or Enter          ", 22, 14,12 ,memory) ;
		return 0;
	}

	store_video ( 5, 5, 20, 70, m ,memory) ;
	menubox ( 6, 5, 11, 60 ,12, 66 ,memory) ;

	writeline("         Change Directory Service                    ",22,14,12,memory);
	writeline("Enter name of directory :",7,8,12,memory);
	size ( 6, 7 ) ;
	gotoxy ( 34, 8 ) ;
	gets ( dname ) ;
	size ( 32, 0 ) ;

	/* issue interrupt for removing directory*/
	iregs.h.ah = 59 ;
	iregs.x.dx = ( unsigned int ) dname ;
	intdos ( &iregs, &oregs ) ;

	/* check if successful in changing directory*/
	if ( oregs.x.cflag == 0 )
		writeline ( "Directory was changed!", 9, 8, 12 ,memory) ;
	else

	{
		if (oregs.x.ax == 5);
		writeline("Improper acces",9,8,12,memory);
		if ( oregs.x.ax == 3 )
			writeline ( "Invalid path!", 9, 8, 12,memory ) ;
	}
	writeline("            Press any key to continue                     ",22,14,12,memory);
	getch();
	writeline("        Select using hotkey or Enter             ",22,14,12,memory);
	restore_video(5,5,20,70,m,memory);
	free(m);
}

// Function to delete the directory
dir_class :: delete_dir(char far *memory)
{
	union  REGS  iregs,oregs;
	int area ;
	char dname[20], *m ;

	area = ( 20 - 5 + 1 ) * ( 76 - 5 + 1 ) * 2 ;
	m =((char *) malloc ( area )) ;
	if ( m == NULL )
	{
		writeline ("      Insufficient memory          ", 22, 14, 12 ,memory) ;
		getch() ;

		writeline ("        Select using hotkey or Enter            ", 22, 14,12 ,memory) ;
		return 0;
	}

	store_video ( 5, 5, 20, 76, m ,memory) ;
	menubox ( 6, 5, 12, 75 ,112, 66 ,memory) ;

	writeline("         Delete Directory Service                    ",22,14,12,memory);
	writeline("Enter name of directory :",7,8,12,memory);
	writeline(" Enter path ",8,8,12,memory);
	size ( 6, 7 ) ;
	gotoxy ( 35, 8 ) ;
	gets ( dname ) ;
	size ( 32, 0 ) ;

	/* issue interrupt for changing directory*/
	iregs.h.ah = 58 ;
	iregs.x.dx = ( unsigned int ) dname ;
	intdos ( &iregs, &oregs ) ;

	/* check if successful in changing directory*/
	if ( oregs.x.cflag == 0 )
		writeline ( "Directory was removed!", 9, 8, 12 ,memory) ;
	else
	{
		switch ( oregs.x.ax )
		{
			case 3 :
				writeline ( "Invalid path!", 10, 8, 112 ,memory) ;
				break ;

			case 5 :
				writeline ( "Improper access!",10, 8, 112,memory ) ;
				break ;

			case 2 :
				writeline ( "Directory does not exist!", 10, 8, 112 ,memory) ;
				break ;

			case 0x10 :
				writeline ( "Cannot remove current directory!", 10, 8, 112 ,memory) ;
		}
	}

	writeline ("             Press any key to continue                 ", 22, 14, 112 ,memory) ;
	getch() ;

	writeline ( "            Select using hotkey or Enter                ",22, 14, 112 ,memory) ;
	restore_video ( 5, 5, 20, 76, m,memory ) ;
	free ( m ) ;
}

// Function to list the directory
dir_class::list_dir(char far *memory)
{

	int area ;
	char *m, fname[20] ;
	char sz[10], d[10], mm[10], y[10], hr[10], minute[10], temp[3] = "0" ;
	struct ffblk file;
	unsigned int find,row,col,a,year,month,day,hour,min;

	area=(20-3+1)*(70-3+1)*2;
	m=((char*)malloc(area));
	if(m==NULL)
	  {
		writeline("     Press any key to continue     ",22,14,12,memory);
		getch() ;

		writeline ( "          Select using hotkey or Enter              ", 22, 14, 12 ,memory) ;
		return 0;
	}

	store_video ( 5, 5, 20, 70, m ,memory) ;
	menubox ( 6, 5, 11, 60, 12, 66 ,memory) ;

	writeline ( "               List directory service               ", 22, 15, 12 ,memory) ;

	writeline ( "Enter skeleton for searching:", 7, 8, 12 ,memory) ;
	size ( 6, 7 ) ;
	gotoxy ( 39, 8 ) ;
	gets ( fname ) ;
	size ( 32, 0 ) ;

	/* find first file which matches the skeleton */
	find = findfirst ( fname, &file, FA_DIREC ) ;

	/* if successful in finding the first file */
	if ( find == 0 )
	{
		menubox ( 5, 5, 20, 70, 12, 66,memory);
		row = 8;
		col =8;
		writeline("Directory ",6,28,12,memory);
		writeline("**********",7,28,12,memory);

		/* carry out search for rest of the files matching the skeleton */
		while ( find == 0 )
		{
			row++ ;
			writeline ( file.ff_name, row, col, 12,memory ) ;

			/* if not a sub-directory entry */
			if ( ( file.ff_attrib & 16 ) == 0 )
			{
				ltoa ( file.ff_fsize, sz, 10 ) ;
				writeline ( sz, row, col + 19, 12 ,memory) ;

				/* calculate and print date and time */
				a = file.ff_fdate ;

				year = 80 + ( a >> 9);
				month = ( a << 7) >> 12;
				day = ( a << 11 ) >>1;

				itoa(day,d,10);

				/* if a single digit day, concatenate it to 0 */
				if ( strlen ( d ) == 1 )
					strcat ( temp, d ) ;
				else
					strcpy ( temp, d ) ;

				writeline ( temp, row, col + 30, 12,memory ) ;
				writech ( row, col + 32, '/', 12 ,memory) ;

				/* reinitialise temp */
				strcpy ( temp, "0" ) ;

				itoa ( month, mm, 10 ) ;

				/* if a single digit month, concatenate it to 0 */
				if ( strlen ( mm ) == 1 )
					strcat ( temp, mm ) ;
				else
					strcpy ( temp, mm ) ;

				writeline ( temp, row, col + 33, 12 ,memory) ;
				writech ( row, col + 35, '/', 12 ,memory) ;

				strcpy ( temp, "0" ) ;

				itoa ( year, y, 10 ) ;
				if ( strlen ( y ) == 1 )
					strcat ( temp, y ) ;
				else
					strcpy ( temp, y ) ;

				writeline ( temp, row, col + 36, 12,memory ) ;

				strcpy ( temp, "0" ) ;

				a = file.ff_ftime ;
				hour = ( a >> 11 ) ;
				min = ( a << 5 ) >> 10 ;

				if ( hour == 0 )
					hour = 12 ;

				strcpy ( temp, "0" ) ;

				itoa ( hour, hr, 10 ) ;
				if ( strlen ( hr ) == 1 )
					strcat ( temp, hr ) ;
				else
					strcpy ( temp, hr ) ;

				writeline ( temp, row, col + 45, 12,memory ) ;
				writech ( row, col + 47, ':', 12,memory ) ;

				strcpy ( temp, "0" ) ;

				itoa ( min, minute, 10 ) ;
				if ( strlen ( minute ) == 1 )
					strcat ( temp, minute ) ;
				else
					strcpy ( temp, minute ) ;

				writeline ( temp, row, col + 48, 12,memory ) ;

				strcpy ( temp, "0" ) ;
			}
			else
				writeline ( "<DIR>", row, col + 15, 12,memory ) ;

			/* find the next file matching the skeleton */
			find = findnext ( &file ) ;

			/* if screen is full */
			if ( row == 18 )
			{
				row = 8 ;
				writeline ( "          Press any key to continue                ", 22, 14, 12 ,memory) ;
				getch() ;

				menubox ( 5, 5, 20, 70, 12, 66 ,memory) ;
				writeline ( "Directory listing", 6, 28, 12,memory ) ;
				writeline ( "ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ", 7, 28, 12,memory ) ;
			}
		}
	}
	else
		writeline ( "File not found!", 9, 8, 12 ,memory) ;

	writeline ("            Press any key to continue                  ", 22, 14, 12 ,memory) ;
	getch() ;

	writeline ( "          Select using hotkey or Enter              ", 22, 14, 12 ,memory) ;
	restore_video ( 5, 5, 20, 70, m,memory ) ;
	free ( m ) ;
}



/* various menu definitions */
/* character following ^ symbol is the hot key */
char *main_menu[] =	{
					"^Directory services",
					"^File services",
					"^Exit"
				} ;

char *f_services[] = {
						"^Compress file",
						"Deco^Mpress file",
						"^Get file attributes",
						"^Set file attributes",
						"^Encrypt file",
						"^Decrypt file",
						"^Type file",
						"de^Lete file",
						"^Return to main menu",
					 } ;

char *d_services[] = {
						"^Make directory",
						"^Change directory",
						"^Delete directory",
						"^List directory",
						"^Return to main menu"
					 } ;


/* help messages for different menu items */
char *comment[] =	{
						" Utility ",
						"           Select using hotkey or Enter            ",
						"Handle various file services",
						"Handle various directory services",
						"Return to DOS",
						"Compress text file",
						"Decompress a compressed file",
						"Display  file attributes",
						"Change  file attributes",
						"Code a file ",
						"Decode a coded file",
						"Display contents of file",
						"Delete a file",
						"Return to main menu",
						"Create a new directory ",
						"Change default directory",
						"Delete an existing directory",
						"List existing directory contents",
						"Return to main menu",
						"           Press any key to continue...             ",
						"      Insufficient space! Press any key...          "
					} ;

char far *memory ;
int ascii, scans ;
set_mode(int);
pop_menu ( char **main_menu, int count, int sr, int sc, char *hotkeys, int helpnumber );
showmenu ( char **main_menu, int count, int sr, int sc );
fserver();
dserver();
delete_file();

getoutput ( char **menu, char *hotkeys, int sr, int sc, int count, int helpnumber );
intro();
mainmenu();
presskey();

display_dir() ;
memsize() ;

listdir();

file_class  file;
dir_class   dir;

/* request auto detection */
int gdriver = DETECT, gmode, errorcode;

/* initialize graphics mode */

main()
{
	int choice ;

	/* store base address of VDU memory and set appropriate video mode */
	#ifdef MA

		/* store base address for MA */
		memory = ( char far * ) 0xb0000000L ;
		set_mode ( 7 ) ;

	#else

		/* store base address for other display adapters */
		memory = ( char far * ) 0xb8000000L ;
		set_mode ( 3 ) ;

	#endif

	size ( 32, 0 ) ;  /* hide cursor */

	/* create opening screen display */
	menubox ( 0, 0, 24, 79, 7, 7,memory ) ;
	display_box ( 1, 0, 21, 79, 7 ,memory) ;

/*	initgraph(&gdriver, &gmode, "");
	logo(memory) ;

	// Shuts down the graphics system
	closegraph();*/


	/* create screen on which menus are to be popped up */
	mainmenu() ;

	while ( 1 )
	{
		/* pop up main menu and collect choice */
		choice = pop_menu ( main_menu, 3, 7, 3, "DFE", 2 );

		/* test choice received */
		switch ( choice )
		{
			case 1 :
				dserver() ;
				break ;

			case 2 :
				fserver() ;
				break ;

			case 3 :
			case 27 :
				size ( 6, 7 ) ;
				clrscr() ;
				exit ( 1 ) ;
		}
	}
}

/* sets video mode*/
set_mode ( int mode )
{
	union REGS i, o ;

	i.h.ah = 0 ;  /* service no.*/
	i.h.al = mode ;  /* video mode*/
	int86 ( 16, &i, &o ) ;  /* issue interrupt*/
}
/* prepares the screen for popping up a menu*/
mainmenu()
{
	int i, j ;

	display_box ( 1, 0, 23, 79, 7,memory ) ;
	display_box ( 5, 0, 21, 79, 7,memory ) ;

	writech ( 5, 0, 204, 7,memory ) ;   //draw boundaries of display boxes
	writech ( 5, 79, 185, 7,memory ) ;
	writech ( 21, 0, 204, 7,memory ) ;
	writech ( 21, 79, 185, 7,memory ) ;

	for ( i = 6 ; i <= 20 ; i++ )
	{
		for ( j = 1 ; j <= 78 ; j += 2 )
		{
			writech ( i, j, 177, 12,memory ) ;
			writech ( i, j + 1, 177, 12,memory ) ;
		}
	}

	writeline("±±",2,4,1,memory);      //char(177)=  ±
	writeline("±±",3,4,1,memory);
	writeline("±±±±±",4,4,1,memory);
	     writeline("±±",2,7,1,memory);
	     writeline("±±",3,7,1,memory);


	writeline("±±±±±±",2,16,1,memory);
	  writeline("±±",3,18,1,memory);
	  writeline("±±",4,18,1,memory);

	writeline("±±",2,29,1,memory);
	writeline("±±",3,29,1,memory);
	writeline("±±",4,29,1,memory);

	writeline("±±",2,38,1,memory);
	writeline("±±",2,38,1,memory);
	writeline("±±",3,38,1,memory);
	writeline("±±±±±",4,38,1,memory);


	writeline("±±",2,49,1,memory);
	writeline("±±",3,49,1,memory);
	writeline("±±",4,49,1,memory);

	writeline("±±±±±±",2,57,1,memory);
	  writeline("±±",3,59,1,memory);
	  writeline("±±",4,59,1,memory);

	writeline("±±",2,70,1,memory);
	    writeline("±±",2,74,1,memory);
	 writeline("±±±±",3,71,1,memory);
	  writeline("±±",4,72,1,memory);

	writeline ( comment[1], 22, 14, 12,memory ) ;
}

/* pops up a menu on the existing screen contents */
pop_menu ( char **main_menu, int count, int sr, int sc, char *hotkeys, int number )
{
	int er, ec, i, l = 0, area, areaforhelp, choice ;
	char *m, *n ;

	/* calculate ending row for menu */
	er = sr + count + 2 ;

	/* find longest menu item */
	for ( i = 0 ; i < count ; i++ )
	{
		if ( strlen (main_menu[i] ) > l )
			l = strlen ( main_menu[i] ) ;
	}

	/* calculate ending column for menu */
	ec = sc + l + 3 ;

	/* calculate area required to save screen contents where menu is to be popped up */
	area = ( er - sr + 1 ) * ( ec - sc + 1 ) * 2 ;

	m = ((char *)malloc ( area )) ;  /* allocate memory */

	/* check if allocation is successful*/
	if(m==NULL)
	  {
	    writeline(comment[20],22,14,9,memory);
	    getch();
	    exit(2);
	  }
	store_video ( sr, sc, er, ec, m,memory ) ;

	/* draw filled box with shadow */
	menubox ( sr, sc, er, ec, 277, 1,memory ) ;

	/* display the menu in the filled box */
	showmenu ( main_menu, count, sr + 1, sc + 1 ) ;

	/* calculate area required for help box */
	areaforhelp = ( 9 - 6 + 1 ) * ( 78 - 35 + 1 ) * 2 ;

	n =((char *) malloc ( areaforhelp )) ;
	if ( n == NULL )
	{
		writeline ( comment[20], 22, 14, 12 ,memory) ;
		getch() ;
		exit ( 3 ) ;
	}
	store_video ( 8, 35,11, 74, n ,memory) ;
	menubox ( 8, 35, 11, 74, 277, 1,memory ) ;

	/* display help message */
	writeline ( comment[number], 9, 36, 143,memory ) ;

	/* receive user's choice */
	choice = getoutput (main_menu, hotkeys, sr, sc, count,number ) ;

	/* restore original screen contents */
	restore_video ( sr, sc, er, ec, m,memory ) ;
	restore_video ( 8, 35, 11, 74, n ,memory) ;

	/* free allocated memory */
	free ( m ) ;
	free ( n ) ;

	return ( choice ) ;
}


/* gets the ascii and scan codes of the key pressed */
getkey()
{
	 // A union is similar to a struct, except it allows
	 //  you to define variables that share storage space.

	union REGS iregs, oregs ;

	/* wait till a key is hit */
	while ( ! kbhit() )
		;

	iregs.h.ah = 0 ;  /* service number */

	/* issue interrupt */
	int86 ( 22, &iregs, &oregs ) ;

	scans = oregs.h.ah ;
	ascii = oregs.h.al ;
}

/* displays the menu in box drawn by menubox() */
showmenu ( char **main_menu, int counter, int sr, int sc )
{
	int i ;

	for ( i = 0 ; i < counter ; i++ )
	{
		/* write menu item in VDU memory */
		writeline ( main_menu[i], sr, sc, 9,memory ) ;
		sr++ ;
	}
}


/* receives user's response for the menu displayed */
getoutput ( char **main_menu, char *keys, int sr, int sc, int counter, int number )
{
	int ch = 1, len, hotkeyans ;

	/* calculate number of hot keys for the menu */
	len = strlen ( keys ) ;

	/* highlight first menu item */
	writeline ( main_menu[ch - 1], sr + ch, sc + 1, 12,memory ) ;

	while ( 1 )
	{
		getkey() ;  /* receive key */

		/* if special key is hit */
		if ( ascii == 0 )
		{
			switch ( scans )
			{
				case 80 :  /* down arrow key */

					/* make highlighted item normal */
					writeline ( main_menu[ch - 1], sr + ch, sc + 1, 9,memory ) ;

					ch++ ;
					number++ ;
					break ;

				case 72 :  /* up arrow key */

					/* make highlighted item normal */
					writeline ( main_menu[ch - 1], sr + ch, sc + 1, 9,memory ) ;

					ch-- ;
					number-- ;
					break ;
			}

			/* if highlighted bar is on first item and up arrow key is hit */
			if ( ch == 0 )
			{
				ch = counter ;
				number = number + counter ;
			}

			/* if highlighted bar is on last item and down arrow key is hit */
			if ( ch > counter )
			{
				ch = 1 ;
				number = number - counter ;
			}

			/* highlight the appropriate menu item */
			writeline ( main_menu[ch - 1], sr + ch, sc + 1, 12 ,memory) ;
			menubox ( 8, 35, 11, 74, 277, 1 ,memory) ;

			/* write the corresponding help message */
			writeline ( comment[number], 9, 36, 143,memory ) ;
		}
		else
		{
			if ( ascii == 13 )  /* Enter key */
				return ( ch ) ;

			if ( ascii == 27 )  /* Esc key */
				return ( 27 ) ;

			ascii = toupper ( ascii ) ;
			hotkeyans = 1 ;

			/* check whether hot key has been pressed */
			while ( *keys != '\0' )
			{
				if ( *keys == ascii )
					return ( hotkeyans ) ;
				else
				{
					keys++ ;
					hotkeyans++ ;
				}
			}

			/* reset hotkeys to point to the first character in the string */
			keys = keys - len ;
		}
	}
}

/* pops up File Services menu, receives choice and branches to appropriate function */
fserver()
{
	int f_choice ;

	while ( 2 )
	{
		f_choice = pop_menu ( f_services, 9, 8, 11, "CMGSEDTLR", 5 ) ;

		switch ( f_choice )
		{

			case 1 :
				file.compress_file(memory) ;
				break ;

			case 2 :
				file.decompress_file(memory) ;
				break ;

			case 3:
				 file.get_attb(memory) ;
				 break ;

			case 4:
				 file.set_attb(memory) ;
				 break ;

			case 5 :
				 file.encrypt_file(memory) ;
				 break ;

			case 6:
				 file.decrypt_file(memory) ;
				 break ;


			case 7 :
				file.type_file(memory) ;
				break ;

			case 8 :
				 file.delete_file(memory) ;
				 break ;
			case 9 :
				 return 0;
		}
	}
}

/* pops up Directory Services menu, receives choice and branches to appropriate function */
dserver()
{
	int d_choice ;

	while ( 2 )
	{
		d_choice = pop_menu ( d_services, 5, 8, 11, "MCDLR", 14 ) ;

		switch ( d_choice )
		{
			case 1 :
				 dir.create_dir(memory) ;
				 break ;

			case 2 :
				dir.change_dir(memory) ;
				 break ;

			case 3 :
				dir.delete_dir(memory) ;
				 break ;

			case 4 :
				 dir.list_dir(memory) ;
				 break ;
			case 5 :
				 return 0;
		}
	}
} 