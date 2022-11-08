 #include"dos.h"
union REGS i,o;
struct SREGS sregs;

static int mask[] = {                          //changing mouse cursor
			//mask the screen      //hand cursor
			0xe1ff,
			0xe1ff,
			0xe1ff,
			0xe1ff,
			0xe1ff,
			0xe000,
			0xe000,
			0xe000,
			0x0000,
			0x0000,
			0x0000,
			0x0000,
			0x0000,
			0x0000,
			0x0000,
			0x0000,
			//cursor mask
			0x1e00,
			0x1200,
			0x1200,
			0x1200,
			0x13ff,
			0x1249,
			0x1249,
			0x9001,
			0x9001,
			0x9001,
			0x8001,
			0x8001,
			0x8001,
			0xffff
		     };


static int mask1[] = {   //screen mask         //smiling face cursor
			 0x0000,
			 0x0000,
			 0x0000,
			 0x0000,
			 0x0000,
			 0x0000,
			 0x0000,
			 0x0000,
			 0x0000,
			 0x0000,
			 0x0000,
			 0x0000,
			 0x0000,
			 0x0000,
			 0x0000,
			 0x0000,
			 //cursor mask
			 0x1ff8,
			 0x2004,
			 0x4002,
			 0x8001,
			 0x9e3d,
			 0x9e3d,
			 0x9e3d,
			 0x8001,
			 0x8001,
			 0x9819,
			 0x8c31,
			 0x87e1,
			 0x83c1,
			 0x4002,
			 0x2004,
			 0x1ff8
		      };


static int name[]= {  //cursor containing name          MJ
		      //cursor ask
		      0xc037,
		      0xa051,
		      0x9091,
		      0x8911,
		      0x8611,
		      0x8011,          //MJ
		      0x8011,
		      0x8011,
		      0x8011,
		      0x8011,
		      0x8091,
		      0x8091,
		      0x8091,
		      0x8091,
		      0x80ff,
		      //screen mask
			 0x0000,
			 0x0000,
			 0x0000,
			 0x0000,
			 0x0000,
			 0x0000,
			 0x0000,
			 0x0000,
			 0x0000,
			 0x0000,
			 0x0000,
			 0x0000,
			 0x0000,
			 0x0000,
			 0x0000,
			 0x0000
		   };






void showmouseptr()
{
i.x.ax=1;
int86(0x33,&i,&o);
}

void hidemouseptr()
{
i.x.ax=2;
int86(0x33,&i,&o);
}
void getmousepos(int *button,int *x, int *y)
{
i.x.ax=3;
int86(0x33,&i,&o);
if(o.x.bx&1) // left button clicked
	*button=1;
else if(o.x.bx&2) // right button clicked
	*button=2;
else // nothing pressed
	*button=0;
*x=o.x.cx;
*y=o.x.dx;
}

void changemouseptr()
  {
    i.x.ax=9;
    i.x.bx=5;
    i.x.cx=0;
    i.x.dx=int(name);
    segread(&sregs);
    sregs.es=sregs.ds;
    int86x(0x33,&i,&o,&sregs);

    o.x.ax=1;  //show mouse ptr
    int86(0x33,&i,&o);
  } 