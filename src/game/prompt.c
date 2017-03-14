/* Various functions concering user interaction. */

#include <tank.h>
#include <string.h>

unsigned wait_yesno(void)
{ unsigned k;

  do k=wait_key(); while (k!=RKEY_Y && k!=RKEY_N);

  return(k);
}

void write_panel(void)
{
  long x, y, pg;

  for (pg = 0; pg < num_pages; pg++)
    for (y=0; y<res_y-1; y+=40)
       for (x=0; x<res_x-1; x+=40)
	 blit(panel, screen, 0, 0, x, y+pg*res_y, 40, 40);
}

void message(char *msg)
{ long l=strlen(msg)*6;

  set_black();
  write_panel();
  m13_add_bar((300-l)/2, 85, l+20, 30, 5);
  m13_write_text((320-l)/2, 98, 15, msg);
  set_palette(pal);
}

void message_key(char *msg)
{
  message(msg);
  wait_key();
  clear(screen);
}

long read_string(char *prompt, char *str, long maxlen)
{ static unsigned char qwerty[]="QWERTYUIOP", asdf[]="ASDFGHJKL", zxcv[]="ZXCVBNM";
  static unsigned char numbers[]="1234567890";

  char string[40];
  long l, x, p;
  unsigned k;

  strcpy(string, str);

  l=strlen(prompt)+maxlen+1;
  x=160-l*3;

  write_panel();
  m13_add_bar(x-10, 85, l*6+20, 30, 5);
  m13_write_text(x, 98, 15, prompt);

  x+=6*strlen(prompt);
  p=strlen(str);

  do
  { m13_bar(x, 98, maxlen*6+6, 5, NULL);
    m13_overwrite_text(x, 98, 15, string);
    m13_bar(x+p*6, 98, 5, 5, 0x0a);

    k=wait_key();

    if (k==RKEY_BS && p>0) p--;

    if (p<maxlen)
    { if (k>15 && k<26) string[p]=qwerty[k-16], p++;
      else if (k>29 && k<39) string[p]=asdf[k-30], p++;
      else if (k>43 && k<51) string[p]=zxcv[k-44], p++;
      else if (k>1 && k<12) string[p]=numbers[k-2], p++;
      else if (k==57) string[p]=' ', p++;
    }

    string[p]=0;

  } while (k!=RKEY_ENTER && k!=RKEY_ESC);

  if (k==RKEY_ENTER)
   strcpy(str, string);

  clear(screen);

  return(k==RKEY_ENTER);
}

#define num_black 11
#define num_lines 22
  
void view_file(FILE *f, long start, long end)
{
  uchar *text, ffn[MAX_PATH];
  long y, p, size, l, t;
  PALETTE pl;
  BITMAP *bmp;
  
  static void update_text(void)
  {
    blit(bmp, screen, 0, 0, 0, 0, 320, 200);

    if (p!=0) m13_write_text(2, 2, 255, "\x1e");

    for (l=0, y=10, t=0; l<num_lines && p+t<size; l++, y+=8, t++)
    { strupr(&text[p+t]);
      m13_write_text(10, y, 255, &text[p+t]);

      for (; text[p+t]!=0; t++);
    }

    if (p+t!=size) m13_write_text(2, 10+num_lines*8, 255, "\x1f");
  }

  if (f==0 || (size=end-start)==0)
   return;

  set_black();

  sprintf(ffn, "DATA/BITMAPS.DAT#BLACK%ld.BMP", rnd(num_black)+1);
  bmp = load_bmp(ffn, pl);
  if (bmp == 0) error("Unable to find: %s\n", ffn);
  
  text=allocate(size);
  fseek(f, start, SEEK_SET);
  fread(text, size, 1, f);

  for (l=0; l<size; l++)
   if (text[l]=='\n')
    text[l]=0;

  text[size-1]=0;

  p=0;

  update_text();
  fade_in(pl, 3);

  for (;;)
  { switch (wait_key())
    { case RKEY_PGUP:
      case RKEY_UP:
       for (t=p, l=0; l<num_lines/2 && t>0; t--)
	if (text[t]==0) l++;

       if (t!=0)
        t+=2;

       if (t!=p)
       { p=t;
	 update_text();
       }

       break;

      case RKEY_PGDN:
      case RKEY_DOWN:
       for (t=p, l=0; l<num_lines/2-1 && t<size; t++)
	if (text[t]==0) l++;

       if (t!=p && t<size)
       { p=t;
	 update_text();
       }

       break;

      case RKEY_ESC:
      case RKEY_ENTER:
      case RKEY_SPACE:
       free(text);
       destroy_bitmap(bmp);
       fade_out(3);
       clear(screen);
       return;
    }
  }
}

void display_picture(char *fn)
{
  char ffn[MAX_PATH];
  BITMAP *bmp;
  PALETTE pal;

  sprintf(ffn, "DATA/BITMAPS.DAT#%s", fn);
  bmp = load_bmp(ffn, pal);
  if (bmp == 0) error("Unable to find: %s\n", fn);

  set_black();
  blit(bmp, screen, 0, 0, 0, 0, 320, 200);
  fade_in(pal, 3);
  destroy_bitmap(bmp);
}

