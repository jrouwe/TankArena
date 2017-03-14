/* The flag for capture the flag missions. */

#include <tank.h>

#define f o->d.fl

static void write_flag(objectstruct *o)
{ if (o->target==NULL)
   b_add_layer(o->x-ssx, o->y-ssy, intern[VLAG+f.type].ptr);

  else o->x=o->target->x+f.type*2, o->y=o->target->y+f.type*2;
}

static void mwrite_flag(objectstruct *o, objectstruct *ao)
{
  b_write_text(o->x-ssx-4, o->y-ssy+14, 15, "FLAG P%ld", f.type+1);
}

static void control_flag(objectstruct *o)
{
  if (f.num != game.flag_no[f.type])
  {
    remove_object(o);
    return;
  }

  if (o->target!=NULL)
  {
    if (o->target->d.tk.und.in_base && o->target->d.tk.contr.num!=f.type && game.winner==-1)
    {
      game.winner=f.type^1;
      game.end_round=tinit(300);
    }

    if (o->target->d.tk.shield.armor==0)
      o->target=NULL;
  }
}

objectstruct *make_flag(long x, long y, long type)
{ objectstruct *o;

  o=create_object(FLAG_TYPE, x, y, 0, 4);

  o->funct.write=write_flag;
  o->funct.mwrite=mwrite_flag;
  o->funct.control=control_flag;

  f.type=type;

  return(o);
}

void prepare_flag(objectstruct *o)
{
  o->funct.write = write_flag;
  o->funct.mwrite = mwrite_flag;

  /* Give each flag a number */

  f.num = game.flag_no[f.type];
  game.flag_no[f.type]++;

  if (game.mode == DUAL || game.mode == MAP)
    {
      o->funct.control = control_flag;

      game.flag_hunt=TRUE;
    }
  else
    o->funct.control = remove_object;
}
