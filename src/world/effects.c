/* Controls all effects in the game (explosions etc.) */

#include <tank.h>

effectstype eff[]={{"DUST1", 25, FALSE, NEXT},
		   {"DUST2", 25, FALSE, NEXT},
		   {"DUST3", 25, FALSE, END},
		   {"SMOKE1", 50, FALSE, NEXT},
		   {"SMOKE2", 50, FALSE, END},
		   {"RUIN1", 25, FALSE, NEXT},
		   {"RUIN2", 25, FALSE, NEXT},
		   {"RUIN3", 25, FALSE, NEXT},
		   {"RUIN4", 25, FALSE, END},
		   {"FIRE1", 13, TRUE, NEXT},
		   {"FIRE2", 25, TRUE, NEXT},
		   {"FIRE3", 25, TRUE, END},
		   {"EXP1", 25, TRUE, NEXT},
		   {"EXP2", 25, TRUE, NEXT},
		   {"EXP3", 25, TRUE, NEXT},
		   {"EXP4", 25, TRUE, NEXT},
		   {"EXP5", 25, TRUE, END},
		   {"CHAIN1", 13, FALSE, NEXT},
		   {"CHAIN2", 13, FALSE, NEXT},
		   {"CHAIN3", 13, FALSE, END},
		   {"FLAME1", 25, TRUE, NEXT},
		   {"FLAME2", 25, TRUE, NEXT},
		   {"FLAME3", 25, TRUE, NEXT},
		   {"FLAME4", 25, TRUE, NEXT},
		   {"FLAME5", 25, TRUE, END},
		   {"EXHAUST1", 100, FALSE, NEXT},
		   {"EXHAUST2", 25, FALSE, END},
		   {"BLOOD1", 50, FALSE, NEXT},
		   {"BLOOD2", 50, FALSE, NEXT},
		   {"BLOOD3", 50, FALSE, END},
		   {"PLUF1", 13, FALSE, NEXT},
		   {"PLUF2", 13, FALSE, END},
		   {"SPLASH1", 25, FALSE, NEXT},
		   {"SPLASH2", 25, FALSE, NEXT},
		   {"SPLASH3", 25, FALSE, END},
		   {"SFIRE", 10, TRUE, END},
		   {"OLIE1", 25, FALSE, NEXT},
		   {"OLIE2", 25, FALSE, NEXT},
		   {"OLIE3", 25, FALSE, END},
		   {"ROOK1", 25, FALSE, NEXT},
		   {"ROOK2", 25, FALSE, NEXT},
		   {"ROOK3", 25, FALSE, NEXT},
		   {"ROOK4", 25, FALSE, NEXT},
		   {"ROOK5", 25, FALSE, END},
		   {"TEL1", 30, FALSE, NEXT},
		   {"TEL2", 30, FALSE, NEXT},
		   {"TEL3", 30, FALSE, NEXT},
		   {"TEL4", 30, FALSE, NEXT},
		   {"TEL5", 30, FALSE, NEXT},
		   {"TEL6", 30, FALSE, END},
		   {"SMOKES1", 50, FALSE, NEXT},
		   {"SMOKES2", 50, FALSE, NEXT},
		   {"SMOKES3", 50, FALSE, NEXT},
		   {"SMOKES4", 50, FALSE, NEXT},
		   {"SMOKES5", 200, FALSE, NEXT},
		   {"SMOKES6", 200, FALSE, NEXT},
		   {"SMOKES5", 200, FALSE, NEXT},
		   {"SMOKES6", 200, FALSE, NEXT},
		   {"SMOKES5", 200, FALSE, NEXT},
		   {"SMOKES6", 200, FALSE, NEXT},
		   {"SMOKES5", 200, FALSE, NEXT},
		   {"SMOKES6", 200, FALSE, NEXT},
		   {"SMOKES5", 200, FALSE, NEXT},
		   {"SMOKES6", 200, FALSE, NEXT},
		   {"SMOKES5", 200, FALSE, NEXT},
		   {"SMOKES6", 200, FALSE, NEXT},
		   {"SMOKES5", 200, FALSE, NEXT},
		   {"SMOKES6", 200, FALSE, NEXT},
		   {"SMOKES5", 200, FALSE, NEXT},
		   {"SMOKES6", 200, FALSE, NEXT},
		   {"SMOKES5", 200, FALSE, NEXT},
		   {"SMOKES6", 200, FALSE, NEXT},
		   {"SMOKES5", 200, FALSE, NEXT},
		   {"SMOKES4", 50, FALSE, NEXT},
		   {"SMOKES3", 50, FALSE, NEXT},
		   {"SMOKES2", 50, FALSE, NEXT},
		   {"SMOKES1", 50, FALSE, END},
		   {"IXP1", 25, TRUE, NEXT},
		   {"IXP2", 25, TRUE, NEXT},
		   {"IXP3", 25, TRUE, NEXT},
		   {"IXP4", 25, TRUE, NEXT},
		   {"IXP5", 25, TRUE, END},
		   {"MFIRE1", 13, TRUE, NEXT},
		   {"MFIRE2", 25, TRUE, NEXT},
		   {"MFIRE3", 25, TRUE, END},
		   {"LAVA1", 25, TRUE, NEXT},
		   {"LAVA2", 25, TRUE, NEXT},
		   {"LAVA3", 25, TRUE, NEXT},
		   {"LAVA4", 25, TRUE, END},
		   {"PARTA1", 13, FALSE, NEXT},
		   {"PARTA2", 13, FALSE, NEXT},
		   {"PARTA3", 13, FALSE, NEXT},
		   {"PARTA4", 13, FALSE, NEXT},
		   {"PARTA5", 13, FALSE, END},
		   {"PARTB1", 13, FALSE, NEXT},
		   {"PARTB2", 13, FALSE, NEXT},
		   {"PARTB3", 13, FALSE, NEXT},
		   {"PARTB4", 13, FALSE, NEXT},
		   {"PARTB5", 13, FALSE, END},
		   {"PARTC1", 13, FALSE, NEXT},
		   {"PARTC2", 13, FALSE, NEXT},
		   {"PARTC3", 13, FALSE, NEXT},
		   {"PARTC4", 13, FALSE, NEXT},
		   {"PARTC5", 13, FALSE, END},
		   {"PARTD1", 13, FALSE, NEXT},
		   {"PARTD2", 13, FALSE, NEXT},
		   {"PARTD3", 13, FALSE, NEXT},
		   {"PARTD4", 13, FALSE, NEXT},
		   {"PARTD5", 13, FALSE, END},
		   {"PARTE1", 13, FALSE, NEXT},
		   {"PARTE2", 13, FALSE, NEXT},
		   {"PARTE3", 13, FALSE, NEXT},
		   {"PARTE4", 13, FALSE, NEXT},
		   {"PARTE5", 13, FALSE, END},
		   {"PARTF1", 13, FALSE, NEXT},
		   {"PARTF2", 13, FALSE, NEXT},
		   {"PARTF3", 13, FALSE, NEXT},
		   {"PARTF4", 13, FALSE, NEXT},
		   {"PARTF5", 13, FALSE, END},
		   {"PARTS1", 13, FALSE, NEXT},
		   {"PARTS2", 13, FALSE, NEXT},
		   {"PARTS3", 13, FALSE, END},
		   {""}};

effectstruct *effect=NULL;
long max_effects=1000;

static void add_effect_to_list(effectstruct *e)
{ effectstruct *p;

  max_effects--;

  if (effect==NULL || e->h<=effect->h)
  { e->prev=NULL;
    e->next=effect;

    if (effect!=NULL)
     effect->prev=e;

    effect=e;

    return;
  }

  for (p=effect; p!=NULL; p=p->next)
   if (e->h<=p->h)
   { e->prev=p->prev;
     e->next=p;
     p->prev->next=e;
     p->prev=e;
     return;
   }

  for (p=effect; p->next!=NULL; p=p->next);

  e->prev=p;
  e->next=NULL;
  p->next=e;
}

static void remove_effect_from_list(effectstruct *e)
{ max_effects++;

  if (e->prev!=NULL)
  { e->prev->next=e->next;

    if (e->next!=NULL)
     e->next->prev=e->prev;
  }
  else
  { effect=e->next;

    if (effect!=NULL)
     effect->prev=NULL;
  }
}

void clear_effects(void)
{ effectstruct *e;

  while(effect!=NULL)
  { max_effects++;

    e=effect, effect=effect->next;
    free(e);
  }
}

void create_effect(long x, long y, long h, long pn)
{ effectstruct *e;

  if (max_effects>0)
  { e=allocate(sizeof(effectstruct));
    e->x=x, e->y=y, e->h=h, e->pn=pn, e->done=tinit(eff[pn].delay);
    add_effect_to_list(e);
  }
}

void control_effects(void)
{ effectstruct *e, *ne;

  for (e=effect; e!=NULL; e=ne)
  { ne=e->next;

    if (tdone(e->done))
     if (eff[e->pn].next==NEXT)
     { e->done=tinit(eff[e->pn].delay+rnd(eff[e->pn].delay)/2);
       e->pn++;
     }
     else
     { remove_effect_from_list(e);
       free(e);
     }
  }
}
