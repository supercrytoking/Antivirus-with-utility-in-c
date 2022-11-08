 /* writes a character and its attribute in VDU memory*/
writech ( int r, int c, char ch, int attb,char far *memory )
{
	char far *v ;

	v = memory + r * 160 + c * 2 ;  /* calculate address in VDU memory corresponding to row r and column c*/

	*v = ch ;  /* store ascii value of character*/
	v++ ;
	*v = attb ;  /* store attribute of character */
}

/* writes a string into VDU memory in the desired attribute */
writeline ( char *s, int r, int c, int attb,char far *memory )
{
	while ( *s != '\0' )
	{
		/* if next character is hot key, write it in different attribute, otherwise in normal attribute*/

		if ( *s == '^' )
		{
			s++ ;
			writech ( r, c, *s, 77,memory ) ;
		}
		else
			writech ( r, c, *s, attb,memory ) ;

		s++ ;
		c++ ;
	}
}
/* draws double-lined box */
display_box ( int sr, int sc, int er, int ec, int attribute,char far *memory )
{
	int i ;

	/* draw horizontal lines */
	for ( i = sc + 1 ; i < ec ; i++ )
	{
		writech ( sr, i, 205, attribute,memory ) ;
		writech ( er, i, 205, attribute,memory ) ;
	}

	/* draw vertical lines */
	for ( i = sr + 1 ; i < er ; i++ )
	{
		writech ( i, sc, 186, attribute,memory ) ;
		writech ( i, ec, 186, attribute,memory ) ;
	}

	/* draw four corners */
	writech ( sr, sc, 201, attribute,memory ) ;
	writech ( sr, ec, 187, attribute,memory ) ;
	writech ( er, sc, 200, attribute,memory ) ;
	writech ( er, ec, 188, attribute,memory ) ;
}
/* draws filled box with or without shadow */
menubox ( int sr, int sc, int er, int ec, char filler, char shadow,char far *memory )
{
	int i, j ;

	/* draw filled box */
	for ( i = sr ; i < er ; i++ )
	{
		for ( j = sc ; j < ( ec - 1 ) ; j++ )
			writech ( i, j, ' ', filler,memory ) ;
	}

	/* if no shadow is required for the filled box */
	if ( shadow == 0 )
	{
		for ( i = sr ; i <= er ; i++ )
		{
			writech ( i, ec, ' ', filler,memory ) ;
			writech ( i, ( ec - 1 ), ' ', filler,memory ) ;
		}

		for ( j = sc ; j <= ec ; j++ )
			writech ( er, j, ' ', filler,memory ) ;
	}
	else
	{
	/* draw vertical and horizontal shadow */
		for ( i = sr + 1 ; i <= er ; i++ )
		{
			writech ( i, ec, ' ', shadow,memory ) ;
			writech ( i, ( ec - 1 ), ' ', shadow,memory ) ;
		}

		for ( j = sc + 2 ; j <= ec ; j++ )
			writech ( er, j, ' ', shadow,memory ) ;
	}
}

/* saves screen contents into allocated memory in RAM */
store_video ( int sr, int sc, int er, int ec, char *buffer,char far *memory )
{
	char far *vid ;
	int i, j ;

	for ( i = sr ; i <= er ; i++ )
	{
		for ( j = sc ; j <= ec ; j++ )
		{
			vid = memory + i * 160 + j * 2 ;  /* calculate address */
			*buffer = *vid ;  /* store character */
			vid++ ;
			buffer++ ;
			*buffer = *vid ;  /* store attribute */
			buffer++ ;
		}
	}
}

/* restores screen contents from allocated memory in RAM */
restore_video ( int sr, int sc, int er, int ec, char *buffer,char far *memory )
{
	char far *vid ;
	int i, j ;

	for ( i = sr ; i <= er ; i++ )
	{
		for ( j = sc ; j <= ec ; j++ )
		{
			vid = memory + i * 160 + j * 2 ;  /* calculate address */
			*vid = *buffer ;  /* restore character */
			vid++ ;
			buffer++ ;
			*vid = *buffer ;  /* restore attribute */
			buffer++ ;
		}
	}
}
 /*displays or hides the cursor*/
size ( int ssl, int esl )
{
	union REGS i, o ;

	i.h.ah = 1 ;  /* service number*/
	i.h.ch = ssl ;  /* starting scan line*/
	i.h.cl = esl ;  /* ending scan line*/
	i.h.bh = 0 ;  /* video page numbe*/

	/* issue interrupt for changing the size of the cursor*/
	int86 ( 16, &i, &o ) ;
}



/* displays the logo on the screen */
logo(char far *memory)
{
	int i, j ;
	setbkcolor(10);
	setcolor(5);
	settextstyle(7,HORIZ_DIR,10);
	outtextxy(60,100,"UTILITY");
	settextstyle(7,HORIZ_DIR,5);
	outtextxy(250,300,"By : ");
	outtextxy(300,400,"Manish Mukhija");
	getch() ;
}
