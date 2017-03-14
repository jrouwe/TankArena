/* Here are all the palette effects (moving water etc.) and functions to
   fade in/out during gameplay. Also a function to read my old palette
   format into allegro palette formats and something for doing gamma
   correction is here.
*/

#include <tank.h>
#include <math.h>

extern PALETTE pal_orig, night_pal_orig;

long nuke_total, palette_update, gamcor = 0;

static long fading=FALSE, fade_timer, fade_dir;
static PALETTE fade_pal;
static float fade_status;
static const float gam[]={1.8, 1.4, 1.1, 1.0, 0.8, 0.6, 0.4};

#define fade_time 0.6

void read_palette(char *fn, PALETTE *p)
{
  FILE *f;
  int i;

  f = open_file(fn);

  for (i = 0; i < 256; i ++)
    fread((char *)&(*p)[i].r, 3, 1, f);

  fclose(f);
}

void start_fade_out(void)
{ if (!fading)
  { fading=TRUE;
    fade_status=1;
    fade_dir=-1;
    fade_timer=timer;
    get_palette(fade_pal);
  }
}

void start_fade_in(PALETTE *p)
{ if (!fading)
  { fading=TRUE;
    fade_status=0;
    fade_dir=1;
    fade_timer=timer;
    memcpy(fade_pal, p, sizeof(PALETTE));
  }
}

long do_fade_step(void)
{ PALETTE p;
  long j;

  if (fading)
  {
    fade_status += (float)fade_dir*(timer-fade_timer)/(fade_time*sec);
    fade_timer = timer;

    if (fade_status<0) fade_status=0, fading=FALSE;
    if (fade_status>1) fade_status=1, fading=FALSE;

    for (j=0; j<256; j++)
    {
      p[j].r = fade_status*fade_pal[j].r;
      p[j].g = fade_status*fade_pal[j].g;
      p[j].b = fade_status*fade_pal[j].b;
    }

    if (game.running)
      set_volume(fade_status * sound_volume, (-0.7 * fade_status + 1) * music_volume);

    set_palette(p);
  }

  return(fading || fade_status>0);
}		 

void rotate_palette(PALETTE p, long s, long e)
{
  short r, g, b;
  int i;

  r=p[s].r, g=p[s].g, b=p[s].b;

  for (i = s; i < e; i++)
  { p[i].r=p[i+1].r;
    p[i].g=p[i+1].g;
    p[i].b=p[i+1].b;
  }

  p[e].r=r, p[e].g=g, p[e].b=b;

  if (nuke_total==0)
    set_palette_range(p, s, e, FALSE);
}

void palette_effects(void)
{
  static long water=tnow, rolling=tnow, teleport=tnow, glow=tnow, lava=tnow;
  PALETTE p, *q=map.night? &night_pal:&pal;
  long i;
  
  if (fading) return;

  if (palette_update)
  { for (i=0; i<256; i++)
    { p[i].r=(*q)[i].r+nuke_total>63? 63:(*q)[i].r+nuke_total;
      p[i].g=(*q)[i].g+nuke_total>63? 63:(*q)[i].g+nuke_total;
      p[i].b=(*q)[i].b+nuke_total>63? 63:(*q)[i].b+nuke_total;
    }

    set_palette(p);

    palette_update=FALSE;
  }
  else
  { if (tdone(water))
    { rotate_palette(*q, 144, 149);
      water=tinit(100+rnd(100));
    }

    if (tdone(rolling))
    { rotate_palette(*q, 150, 153);
      rolling=tinit(10);
    }

    if (tdone(teleport))
    { rotate_palette(*q, 154, 158);
      teleport=tinit(20);
    }

    if (tdone(glow))
    { rotate_palette(*q, 159, 164);
      glow=tinit(50);
    }

    if (tdone(lava))
    { rotate_palette(*q, 165, 170);
      lava=tinit(50+rnd(100));
    }
  }
}

static void correct_pal(PALETTE d, PALETTE s)
{
  float g = gam[gamcor+3];
  long i;

  for (i=0; i<256; i++)
  {
    d[i].r = pround(pow(63, 1 - g) * pow(s[i].r, g));
    d[i].g = pround(pow(63, 1 - g) * pow(s[i].g, g));
    d[i].b = pround(pow(63, 1 - g) * pow(s[i].b, g));
  }
}

void do_gamma_correction(void)
{
  correct_pal(pal, pal_orig);
  correct_pal(night_pal, night_pal_orig);

  palette_update = TRUE;
}
