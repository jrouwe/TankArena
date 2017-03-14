/* Function to compute if tank overlaps with building. */

#include <tank.h>
#include <math.h>

#define tnk o->d.tk

long overlap(objectstruct *o, uchar *t, long x, long y, long *bx, long *by)
{ long sx, sy, ov=0;

  sx=(x+b_size)/b_size-1, x=(x+b_size)%b_size;
  sy=(y+b_size)/b_size-1, y=(y+b_size)%b_size;

  *bx=sx, *by=sy;

  if (o->h<2)
  { if (check(sx, sy) && strength[0][0][sx+map.sx*sy]>0)
     ov+=overlap_rb(t, x, y, pic[mapm.m[1][sx+map.sx*sy]].ptr[0]);

    if (ov==0) (*bx)++;
    sx++;

    if (x>0 && check(sx, sy) && strength[0][0][sx+map.sx*sy]>0)
     ov+=overlap_lb(t, x, y, pic[mapm.m[1][sx+map.sx*sy]].ptr[0]);

    if (ov==0) (*bx)--, (*by)++;
    sx--, sy++;

    if (y>0 && check(sx, sy) && strength[0][0][sx+map.sx*sy]>0)
     ov+=overlap_rt(t, x, y, pic[mapm.m[1][sx+map.sx*sy]].ptr[0]);

    if (ov==0) (*bx)++;
    sx++;

    if (x>0 && y>0 && check(sx, sy) && strength[0][0][sx+map.sx*sy]>0)
     ov+=overlap_lt(t, x, y, pic[mapm.m[1][sx+map.sx*sy]].ptr[0]);
  }

  else if (o->h<8)
  { if (check(sx, sy) && strength[0][0][sx+map.sx*sy]>0 && mapm.m[2][sx+map.sx*sy]!=-1)
     ov+=overlap_rb(t, x, y, pic[mapm.m[2][sx+map.sx*sy]].ptr[strength[1][0][sx+map.sx*sy]==0? 1:0]);

    if (ov==0) (*bx)++;
    sx++;

    if (x>0 && check(sx, sy) && strength[0][0][sx+map.sx*sy]>0 && mapm.m[2][sx+map.sx*sy]!=-1)
     ov+=overlap_lb(t, x, y, pic[mapm.m[2][sx+map.sx*sy]].ptr[strength[1][0][sx+map.sx*sy]==0? 1:0]);

    if (ov==0) (*bx)--, (*by)++;
    sx--, sy++;

    if (y>0 && check(sx, sy) && strength[0][0][sx+map.sx*sy]>0 && mapm.m[2][sx+map.sx*sy]!=-1)
     ov+=overlap_rt(t, x, y, pic[mapm.m[2][sx+map.sx*sy]].ptr[strength[1][0][sx+map.sx*sy]==0? 1:0]);

    if (ov==0) (*bx)++;
    sx++;

    if (x>0 && y>0 && check(sx, sy) && strength[0][0][sx+map.sx*sy]>0 && mapm.m[2][sx+map.sx*sy]!=-1)
     ov+=overlap_lt(t, x, y, pic[mapm.m[2][sx+map.sx*sy]].ptr[strength[1][0][sx+map.sx*sy]==0? 1:0]);
  }

  return(ov);
}

