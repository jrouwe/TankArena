/* This file was based upon the setup program from Allegro 3.0, it controls
   the sound setup.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "allegro.h"
#include "internal.h"
#include "setsound.h"

#define TITLE		"Tank Arena Sound Setup"
#define CFG_FILE        "tank.cfg"

static DATAFILE *data;

#define background  (BITMAP *)data[BACKGROUND].dat
#define setup_font  (FONT *)data[SETUP_FONT].dat
#define setup_pal   (PALLETE *)data[SETUP_PAL].dat
#define test_sample (SAMPLE *)data[TEST_SAMPLE].dat

/* info about a hardware driver */
typedef struct SOUNDCARD
{
   int id;
   char *name;
   char **param;
   char *desc;
   int present;
} SOUNDCARD;
	   
SOUNDCARD *soundcard;
	   
/* list which parameters are used by each driver */
char *digi_auto_param[]    = { "11", "digi_volume", NULL };
char *digi_sbm_param[]     = { "sb_port", "sb_dma", "sb_irq", "digi_volume", "digi_voices", "sb_freq", NULL };
char *digi_sbs_param[]     = { "sb_port", "sb_dma", "sb_irq", "flip_pan", "", "sb_freq", "digi_volume", "digi_voices", NULL };

char *midi_auto_param[]    = { "11", "midi_volume", NULL };
char *midi_adlib_param[]   = { "22", "fm_port", "midi_volume", "", "ibk_file", "ibk_drum_file", NULL };
char *midi_sb_param[]      = { "21", "sb_port", "midi_volume", NULL };
char *midi_mpu_param[]     = { "21", "mpu_port", "midi_volume", NULL };
char *midi_digmid_param[]  = { "22", "midi_voices", "midi_volume", "", "patches", NULL };
char *midi_awe_param[]     = { "21", "midi_voices", "midi_volume", NULL };
	   
/* list of digital sound drivers */
SOUNDCARD digi_cards[] =
{
   /* id                name                    parameter list       desc */
   { DIGI_AUTODETECT,   "Autodetect",           digi_auto_param,     "Attempt to autodetect the digital sound hardware" },
   { DIGI_SB,           "Generic SB",           digi_sbs_param,      "SoundBlaster: autodetect the model" },
   { DIGI_SB10,         "SoundBlaster 1.0",     digi_sbm_param,      "SB v1.0, 8 bit mono using single-shot dma" },
   { DIGI_SB15,         "SoundBlaster 1.5",     digi_sbm_param,      "SB v1.5, 8 bit mono using single shot dma" },
   { DIGI_SB20,         "SoundBlaster 2.0",     digi_sbm_param,      "SB v2.0, 8 bit mono using auto-initialised dma" },
   { DIGI_SBPRO,        "SoundBlaster Pro",     digi_sbs_param,      "SB Pro, 8 bit stereo DAC" },
   { DIGI_SB16,         "SoundBlaster 16",      digi_sbs_param,      "SB16 or AWE32, 16 bit stereo DAC" },
// { DIGI_GUS,          "Gravis Ultrasound",    NULL,                "*** not finished, do not use! ***" },
   { DIGI_NONE,         "No Sound",             NULL,                "The Sound of Silence..." }
};
	   
/* list of MIDI sound drivers */
SOUNDCARD midi_cards[] =
{
   /* id                name                    parameter list       desc */
   { MIDI_AUTODETECT,   "Autodetect",           midi_auto_param,     "Attempt to autodetect the MIDI hardware" },
   { MIDI_ADLIB,        "Generic Adlib",        midi_adlib_param,    "OPL FM synth: autodetect the model" },
   { MIDI_OPL2,         "Adlib (OPL2)",         midi_adlib_param,    "(mono) OPL2 FM synth (used in Adlib and standard SB cards)" },
   { MIDI_2XOPL2,       "Adlib (dual OPL2)",    midi_adlib_param,    "(stereo) Two OPL2 FM synths (early SB Pro cards)" },
   { MIDI_OPL3,         "Adlib (OPL3)",         midi_adlib_param,    "(stereo) OPL3 FM synth (Adlib Gold, later SB Pro boards, SB16)" },
   { MIDI_AWE32,        "AWE32",                midi_awe_param,      "SoundBlaster AWE32 (EMU8000 synth chip)" },
   { MIDI_SB_OUT,       "SB MIDI-OUT",          midi_sb_param,       "Raw SB MIDI output to an external synth module" },
   { MIDI_MPU,          "MPU-401",              midi_mpu_param,      "Raw MPU MIDI output to an external synth module" },
// { MIDI_GUS,          "Gravis Ultrasound",    NULL,                "*** not finished, do not use! ***" },
   { MIDI_DIGMID,       "Digital MIDI",         midi_digmid_param,   "Software wavetable synthesis using the digital sound hardware" },
   { MIDI_NONE,         "No Sound",             NULL,                "The Sound of Silence..." }
};
	   
/* different types of parameter */
typedef enum PARAM_TYPE
{
   param_none,
   param_int,
   param_hex,
   param_bool,
   param_file,
   param_list
} PARAM_TYPE;
	   
/* info about a soundcard parameter */
typedef struct PARAMETER
{
   char *name;
   PARAM_TYPE type;
   char value[80];
   char *def;
   int *detect;
   char *label;
   char *e1, *e2;
   char *desc;
} PARAMETER;
	   
/* list of soundcard parameters */
PARAMETER parameters[] =
{
   /* name              type           value    default     detect            label       extra1      extra2   desc */
   { "digi_card",       param_int,     "",      "-1",       &digi_card,       "",         NULL,       NULL,    "" },
   { "midi_card",       param_int,     "",      "-1",       &midi_card,       "",         NULL,       NULL,    "" },
   { "digi_volume",     param_int,     "",      "255",      NULL,             "Vol:",     NULL,       NULL,    "Digital sound volume (0 to 255)" },
   { "midi_volume",     param_int,     "",      "255",      NULL,             "Vol:",     NULL,       NULL,    "MIDI music volume (0 to 255)" },
   { "digi_voices",     param_int,     "",      "8",        NULL,             "Chan:",    NULL,       NULL,    "Number of channels reserved for playing digital sounds (higher values increase polyphony but degrade speed and quality)" },
   { "midi_voices",     param_int,     "",      "8",        NULL,             "Chan:",    NULL,       NULL,    "Number of channels reserved for playing MIDI music (higher values increase polyphony but degrade speed and quality)" },
   { "flip_pan",        param_bool,    "",      "0",        NULL,             "Pan:",     NULL,       NULL,    "Reverses the left/right stereo placement" },
   { "sb_port",         param_hex,     "",      "-1",       &_sb_port,        "Port:",    NULL,       NULL,    "Port address (usually 220)" },
   { "sb_dma",          param_int,     "",      "-1",       &_sb_dma,         "DMA:",     NULL,       NULL,    "DMA channel (usually 1)" },
   { "sb_irq",          param_int,     "",      "-1",       &_sb_irq,         "IRQ:",     NULL,       NULL,    "IRQ number (usually 7)" },
   { "sb_freq",         param_list,    "",      "-1",       &_sb_freq,        "Freq:",    NULL,       NULL,    "Sample mixing frequency (higher values sound better but require more CPU processing time)" },
   { "fm_port",         param_hex,     "",      "-1",       &_fm_port,        "Port:",    NULL,       NULL,    "Port address (usually 388)" },
   { "mpu_port",        param_hex,     "",      "-1",       &_mpu_port,       "Port:",    NULL,       NULL,    "Port address (usually 330)" },
   { "ibk_file",        param_file,    "",      "",         NULL,             "IBK:",     "IBK",      NULL,    "Custom .IBK instrument patch set" },
   { "ibk_drum_file",   param_file,    "",      "",         NULL,             "drumIBK:", "IBK",      NULL,    "Custom .IBK percussion patch set" },
   { "patches",         param_file,    "",      "",         NULL,             "Patches:", "CFG;DAT",  NULL,    "MIDI patch set (GUS format default.cfg or Allegro format patches.dat)" },
   { NULL,              param_none,    "",      "",         NULL,             NULL,       NULL,       NULL,    NULL } 
};
	   
/* in some places we need to double-buffer the display... */
BITMAP *buffer;
	   
/* dialogs do fancy stuff as they slide on and off the screen */
typedef enum DIALOG_STATE
{
   state_start,
   state_slideon,
   state_active,
   state_slideoff,
   state_exit,
   state_chain,
   state_redraw
} DIALOG_STATE;
	   
/* info about an active dialog */
typedef struct ACTIVE_DIALOG
{
   DIALOG_STATE state;
   int time;
   DIALOG *dialog;
   DIALOG_PLAYER *player;
   BITMAP *buffer;
   DIALOG_STATE (*handler)(int c);
} ACTIVE_DIALOG;
	   
/* list of active dialogs */
ACTIVE_DIALOG dialogs[4];

int dialog_count = 0;
	   
/* scrolly text message at the base of the screen */
volatile int scroller_time = 0;
char scroller_msg[42];
int scroller_pos = 0;
int scroller_alpha = 256;
char *scroller_string = "";
char *wanted_scroller = "";
int scroller_string_pos = 0;
	   
/* timer interrupt handler */
void inc_scroller_time()
{
   scroller_time++;
}

END_OF_FUNCTION(inc_scroller_time);
	   
/* dialog procedure for animating the scroller text */
int scroller_proc(int msg, DIALOG *d, int c)
{
   int redraw = FALSE;
   int a, i, x;

   if (msg == MSG_IDLE) {
      while (scroller_time > 0) {
	 scroller_pos--;
	 if (scroller_pos <= -8) {
	    scroller_pos = 0;
	    for (i=0; i<(int)sizeof(scroller_msg)-1; i++)
	       scroller_msg[i] = scroller_msg[i+1];
	    if (scroller_string[scroller_string_pos])
	       scroller_msg[i] = scroller_string[scroller_string_pos++];
	    else 
	       scroller_msg[i] = ' ';
	    if (wanted_scroller != scroller_string) {
	       scroller_alpha -= MIN(32, scroller_alpha);
	       if (scroller_alpha <= 0) {
		  memset(scroller_msg, ' ', sizeof(scroller_msg));
		  scroller_string = wanted_scroller;
		  scroller_string_pos = 0;
		  for (x=0; x<4; x++) {
		     if (scroller_string[scroller_string_pos]) {
			for (i=0; i<(int)sizeof(scroller_msg)-1; i++)
			   scroller_msg[i] = scroller_msg[i+1];
			scroller_msg[i] = scroller_string[scroller_string_pos];
			scroller_string_pos++;
		     }
		  }
		  scroller_alpha = 256;
	       }
	    }
	    else
	       scroller_alpha += MIN(16, 256-scroller_alpha);
	 }
	 redraw = TRUE;
	 scroller_time--;
      }
   }
   else if (msg == MSG_RADIO) {
      memset(scroller_msg, ' ', sizeof(scroller_msg));
      scroller_string = wanted_scroller;
      scroller_string_pos = strlen(scroller_string);
      scroller_alpha = 256;
      redraw = TRUE;
   }

   if (redraw) {
      freeze_mouse_flag = TRUE;
      text_mode(0);

      for (i=0; i<(int)sizeof(scroller_msg); i++) {
	 x = i*8+scroller_pos;
	 a = MID(0, 15-ABS(SCREEN_W/2-x)/10, 15) * scroller_alpha/16;
	 textprintf(screen, font, x, SCREEN_H-16, a, "%c", scroller_msg[i]);
      }

      freeze_mouse_flag = FALSE;
   }

   return D_O_K;
}
	   
/* helper for drawing a dialog onto a memory bitmap */
void draw_dialog(ACTIVE_DIALOG *d)
{
   BITMAP *oldscreen = screen;
   int nowhere;

   if (d->player->focus_obj >= 0) {
      SEND_MESSAGE(d->dialog+d->player->focus_obj, MSG_LOSTFOCUS, 0);
      d->dialog[d->player->focus_obj].flags &= ~D_GOTFOCUS;
      d->player->focus_obj = -1;
   }

   if (d->player->mouse_obj >= 0) {
      SEND_MESSAGE(d->dialog+d->player->mouse_obj, MSG_LOSTMOUSE, 0);
      d->dialog[d->player->mouse_obj].flags &= ~D_GOTMOUSE;
      d->player->mouse_obj = -1;
   }

   d->player->res &= ~D_WANTFOCUS;

   show_mouse(NULL);
   clear(d->buffer);
   screen = d->buffer; 
   dialog_message(d->dialog, MSG_DRAW, 0, &nowhere);
   show_mouse(NULL);
   screen = oldscreen;
}

/* start up another dialog */
void activate_dialog(DIALOG *dlg, DIALOG_STATE (*handler)(int c), int chain)
{
   ACTIVE_DIALOG *d = &dialogs[dialog_count];

   d->state = state_start;
   d->time = retrace_count;
   d->dialog = dlg;
   d->player = init_dialog(dlg, -1);
   d->buffer = create_bitmap(SCREEN_W, SCREEN_H);
   d->handler = handler;

   draw_dialog(d);

   if (dialog_count > 0) {
      draw_dialog(&dialogs[dialog_count-1]);
      dialogs[dialog_count-1].state = (chain ? state_chain : state_slideoff);
      dialogs[dialog_count-1].time = retrace_count;
   }
}

/* main dialog update routine */
int update()
{
   BITMAP *oldscreen = screen;
   ACTIVE_DIALOG *d;
   DIALOG_STATE state;
   PALETTE pal;
   int pos, ppos, pppos;

   if (dialog_count <= 0)
      return FALSE;

   d = &dialogs[dialog_count-1];

   if (d->state == state_active) {
      /* process the dialog */
      if (!update_dialog(d->player)) {
	 if (d->handler)
	    state = d->handler(d->player->obj);
	 else
	    state = state_exit;

	 if (state == state_exit) {
	    /* exit this dialog */
	    draw_dialog(d);
	    d->state = state_exit;
	    d->time = retrace_count;
	 }
	 else if (state == state_redraw) {
	    /* redraw the dialog */
	    d->player->res |= D_REDRAW;
	 }
      }
      else {
	 pos = find_dialog_focus(d->dialog);
	 if ((pos >= 0) && (d->dialog[pos].dp3))
	    wanted_scroller = d->dialog[pos].dp3;
      }
   }
   else {
      /* sliding on or off */
      blit(background, buffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

      text_mode(0);
      textout_centre(buffer, font, TITLE, SCREEN_W/2, 0, 255);

      wanted_scroller = "";
      screen = buffer;
      scroller_proc(MSG_IDLE, NULL, 0);
      screen = oldscreen;

      pos = retrace_count - d->time;

      if ((dialog_count == 1) && (d->state == state_start))
	 pos *= 64;
      else if ((dialog_count == 1) && (d->state == state_exit))
	 pos *= 48;
      else if (d->state == state_start)
	 pos *= 96;
      else
	 pos *= 128;

      pos = MID(0, 4096-pos, 4096);
      pppos = (4096 - pos * pos / 4096);
      pos = pppos / 16;

      /* draw the slide effect */
      switch (d->state) {

	 case state_start:
	    ppos = pos;
	    stretch_sprite(buffer, d->buffer, 0, 0, SCREEN_W+1024-pppos/4, SCREEN_H+1024-pppos/4);
	    break;

	 case state_slideon:
	    ppos = pos;
	    stretch_sprite(buffer, d->buffer, 0, 0, SCREEN_W*ppos/256, SCREEN_H*ppos/256);
	    break;

	 case state_slideoff:
	 case state_chain:
	    ppos = 256 - pos;
	    stretch_sprite(buffer, d->buffer, SCREEN_W/2-SCREEN_W*ppos/512, SCREEN_H/2-SCREEN_H*ppos/512, SCREEN_W*ppos/256, SCREEN_H*ppos/256);
	    break;

	 case state_exit:
	    ppos = 256 - pos;
	    stretch_sprite(buffer, d->buffer, SCREEN_W-SCREEN_W*ppos/256, SCREEN_H-SCREEN_H*ppos/256, SCREEN_W*ppos/256, SCREEN_H*ppos/256);
	    break;

	 default:
	    ppos = 0;
	    break;
      }

      if ((dialog_count == 1) && (d->state != state_slideon) && (d->state != state_slideoff) && (d->state != state_chain)) {
	 fade_interpolate(black_palette, *setup_pal, pal, ppos/4, 0, 255);
	 set_palette(pal);
      }
      else
	 vsync();

      blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

      if (pos >= 256) {
	 /* finished the slide */
	 switch (d->state) {

	    case state_start:
	    case state_slideon:
	       /* become active */
	       d->state = state_active;
	       d->player->res |= D_REDRAW;
	       break;

	    case state_slideoff:
	       /* activate next dialog, and then get ready to slide back on */
	       dialogs[dialog_count].time = retrace_count;
	       dialog_count++;
	       d->state = state_slideon;
	       break;

	    case state_exit:
	       /* time to die! */
	       shutdown_dialog(d->player);
	       destroy_bitmap(d->buffer);
	       dialog_count--;
	       if (dialog_count > 0)
		  dialogs[dialog_count-1].time = retrace_count;
	       show_mouse(NULL);
	       break;

	    case state_chain:
	       /* kill myself, then activate the next dialog */
	       shutdown_dialog(d->player);
	       destroy_bitmap(d->buffer);
	       dialogs[dialog_count].time = retrace_count;
	       dialogs[dialog_count-1] = dialogs[dialog_count];
	       show_mouse(NULL);
	       break;

	    default:
	       break;
	 }
      }
   }

   return TRUE;
}

/* helper for checking which drivers are valid */
void detect_sound()
{
   int i;

   for (i=0; digi_cards[i].id != DIGI_NONE; i++) {
      if (detect_digi_driver(digi_cards[i].id) == 0)
	 digi_cards[i].present = FALSE;
      else
	 digi_cards[i].present = TRUE;
   }
   digi_cards[i].present = TRUE;

   for (i=0; midi_cards[i].id != MIDI_NONE; i++) {
      if (detect_midi_driver(midi_cards[i].id) == 0)
	 midi_cards[i].present = FALSE;
      else
	 midi_cards[i].present = TRUE;
   }
   midi_cards[i].present = TRUE;
}

/* helper for initialising the sound code */
int init_sound(char *msg)
{
   char b1[80], b2[80];
   int i;

   detect_sound();

   if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) == 0)
      return 0;

   if (strlen(allegro_error) <= 32) {
      strcpy(b1, allegro_error);
      b2[0] = 0;
   }
   else {
      for (i=strlen(allegro_error)*9/16; i>10; i--)
	 if (allegro_error[i] == ' ')
	    break;

      strncpy(b1, allegro_error, i);
      b1[i] = 0;

      strcpy(b2, allegro_error+i+1);
   }

   alert(msg, b1, b2, "Ok", NULL, 0, 0);

   return -1;
}

BITMAP *popup_bitmap = NULL;
BITMAP *popup_bitmap2 = NULL;

/* helper for displaying a popup message */
void popup(char *s1, char *s2)
{
   int w;

   if (!popup_bitmap) {
      for (w=512; w>=0; w-=2) {
	 line(screen, w, 16, 0, 16+w, 0);
	 if (!(w&15))
	    vsync();
      }
      for (w=0; w<512; w+=2) {
	 line(screen, w+1, 16, 0, 17+w, 0);
	 if (!(w&15))
	    vsync();
      }
      popup_bitmap = create_bitmap(SCREEN_W, SCREEN_H);
      popup_bitmap2 = create_bitmap(SCREEN_W, SCREEN_H);
      blit(screen, popup_bitmap, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
   }

   blit(popup_bitmap, popup_bitmap2, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

   w = MAX(text_length(font, s1), text_length(font, s2));

   rectfill(popup_bitmap2, (SCREEN_W-w)/2-15, SCREEN_H/2-31, (SCREEN_W+w)/2+15, SCREEN_H/2+31, 0);
   rect(popup_bitmap2, (SCREEN_W-w)/2-16, SCREEN_H/2-32, (SCREEN_W+w)/2+16, SCREEN_H/2+32, 255);

   text_mode(-1);
   textout_centre(popup_bitmap2, font, s1, SCREEN_W/2, SCREEN_H/2-20, 255);
   textout_centre(popup_bitmap2, font, s2, SCREEN_W/2, SCREEN_H/2+4, 255);

   blit(popup_bitmap2, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
}

/* ends the display of a popup message */
void end_popup()
{
   if (popup_bitmap) {
      blit(popup_bitmap, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
      destroy_bitmap(popup_bitmap);
      popup_bitmap = NULL;
      destroy_bitmap(popup_bitmap2);
      popup_bitmap2 = NULL;
   }
}

/* dialog callback for retrieving the SB frequency list */
char *freq_getter(int index, int *list_size)
{
   static char *freq[] =
   {
      "11906 hz",
      "16129 hz",
      "22727 hz",
      "45454 hz"
   };

   if (index < 0) {
      if (list_size) {
	 switch (atoi(parameters[0].value)) {

	    case DIGI_SB10:
	    case DIGI_SB15:
	       *list_size = 2;
	       break;

	    case DIGI_SBPRO:
	       *list_size = 3;
	       break;

	    case DIGI_SB:
	    case DIGI_SB20:
	    case DIGI_SB16:
	    default:
	       *list_size = 4;
	       break;
	 }
      }
      return NULL;
   }

   return freq[index];
}

/* dialog callback for retrieving information about the soundcard list */
char *card_getter(int index, int *list_size)
{
   static char buf[80];
   int i;

   if (index < 0) {
      i = 0;
      while (soundcard[i].id)
	 i++;
      if (list_size)
	 *list_size = i+1;
      return NULL;
   }

   if (soundcard[index].present)
      sprintf(buf, "%c %s", 251, soundcard[index].name);
   else
      sprintf(buf, "  %s", soundcard[index].name);

   return buf;
}

/* dialog procedure for the soundcard selection listbox */
int card_proc(int msg, DIALOG *d, int c)
{
   int ret = d_list_proc(msg, d, c);
   d->dp3 = soundcard[d->d1].desc;
   return ret;
}

/* dialog procedure for the filename selection objects */
int filename_proc(int msg, DIALOG *d, int c)
{
   PARAMETER *p = d->dp2;
   char buf[256];
   char buf2[256];
   int ret;
   int i;

   if (msg == MSG_START) {
      if (!p->e2)
	 p->e2 = malloc(80);
      strcpy(p->e2, p->value);
   }
   else if (msg == MSG_END) {
      if (p->e2) {
	 free(p->e2);
	 p->e2 = NULL;
      }
   }

   ret = d_check_proc(msg, d, c);

   if (ret & D_CLOSE) {
      if (p->value[0]) {
	 strcpy(p->e2, p->value);
	 p->value[0] = 0;
      }
      else {
	 scroller_proc(MSG_RADIO, NULL, 0);

	 strcpy(buf2, p->desc);

	 for (i=1; buf2[i]; i++) {
	    if (buf2[i] == '(') {
	       buf2[i-1] = 0;
	       break;
	    }
	 }

	 strcpy(buf, p->e2);

	 if (file_select(buf2, buf, p->e1)) {
	    strcpy(p->value, buf);
	    strcpy(p->e2, buf);
	 }

	 show_mouse(NULL);
	 blit(background, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
	 text_mode(0);
	 textout_centre(screen, font, TITLE, SCREEN_W/2, 0, 255);
      }

      if (p->value[0])
	 d->flags |= D_SELECTED;
      else
	 d->flags &= ~D_SELECTED;

      ret &= ~D_CLOSE;
      ret |= D_REDRAW;
   }

   return ret;
}

/* wrapper for d_list_proc() to free up the dp2 parameter */
int d_xlist_proc(int msg, DIALOG *d, int c)
{
   void *old_dp2;
   int ret;

   old_dp2 = d->dp2;
   d->dp2 = NULL;

   ret = d_list_proc(msg, d, c);

   d->dp2 = old_dp2;

   return ret;
}

char backup_str[] =  "Go back to the previous menu";
char midi_desc[160];
char digi_desc[160];

DIALOG main_dlg[] =
{
   /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key)    (flags)     (d1)           (d2)     (dp)                          (p)      (help message) */
   { d_button_proc,	30,   32,   124,  22,	255,   0,   0,	     D_EXIT,	 0,		0,	 "Autodetect",                 NULL,    "Attempt to autodetect your soundcard" },
   { d_button_proc,	166,  32,   124,  22,	255,   0,   0,	     D_EXIT,	 0,		0,	 "Test",                       NULL,    "Test the current settings" },
   { d_button_proc,	30,   60,   124,  22,	255,   0,   0,	     D_EXIT,	 0,		0,	 "Digital Driver",             NULL,    "Manually select a driver for playing digital samples" },
   { d_button_proc,	166,  60,   124,  22,	255,   0,   0,	     D_EXIT,	 0,		0,	 "Midi Driver",                NULL,    "Manually select a driver for playing MIDI music" },
   { d_button_proc,	30,   116,  260,  22,	255,   0,   0,	     D_EXIT,	 0,		0,	 "Save changes and Exit",      NULL,    "Exit from the program, saving the current settings into the file '" CFG_FILE "'" },
   { d_button_proc,	30,   144,  260,  22,	255,   0,   0,	     D_EXIT,	 0,		0,	 "Quit (abandoning changes)",  NULL,    "Exit from the program, without saving the current settings" },
   { scroller_proc,     0,    0,    0,    0,    0,    0,    0,       0,          0,             0,       NULL,                         NULL,    NULL },
   { NULL }
};

DIALOG test_dlg[] =
{
   /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key)    (flags)     (d1)           (d2)     (dp)                          (p)      (help message) */
   { d_button_proc,	100,  50,   120,  24,	255,   0,   0,	     D_EXIT,	 0,		0,	 "MIDI",                       NULL,    midi_desc },
   { d_button_proc,	30,   87,   80,   24,	255,   0,   0,	     D_EXIT,	 0,		0,	 "Left",                       NULL,    digi_desc },
   { d_button_proc,	120,  87,   80,   24,	255,   0,   0,	     D_EXIT,	 0,		0,	 "Centre",                     NULL,    digi_desc },
   { d_button_proc,	210,  87,   80,   24,	255,   0,   0,	     D_EXIT,	 0,		0,	 "Right",                      NULL,    digi_desc },
   { d_button_proc,	100,  124,  120,  24,	255,   0,   0,	     D_EXIT,	 0,		0,	 "Exit",                       NULL,    backup_str },
   { scroller_proc,     0,    0,    0,    0,    0,    0,    0,       0,          0,             0,       NULL },
   { NULL }
};

DIALOG card_dlg[] =
{
   /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key)    (flags)     (d1)           (d2)     (dp)                          (p)      (help message) */
   { d_button_proc,	30,   132,  124,  24,	255,   0,   0,	     D_EXIT,	 0,		0,	 "OK",                         NULL,    "Use this driver" },
   { d_button_proc,	166,  132,  124,  24,	255,   0,   0,	     D_EXIT,	 0,		0,	 "Cancel",                     NULL,    backup_str },
   { card_proc, 	60,   36,   200,  83,	255,  0,   0,	    D_EXIT,	0,	       0,	card_getter,		      NULL,    NULL },
   { scroller_proc,     0,    0,    0,    0,    0,    0,    0,       0,          0,             0,       NULL },
   { NULL }
};

/* this one is generated depending on which parameters we need */
DIALOG param_dlg[32];

int param_ok;

/* handle input from the parameter dialog */
DIALOG_STATE param_handler(int c)
{
   PARAMETER *p;
   DIALOG *d = param_dlg;
   int i;

   if (c == param_ok) {
      /* save the changes */ 
      while (d->proc) {
	 p = d->dp2;

	 if (p) {
	    switch (p->type) {

	       case param_int:
		  if (p->value[0])
		     i = strtol(p->value, NULL, 0);
		  else
		     i = -1;
		  set_config_int("sound", p->name, i);
		  strcpy(p->value, get_config_string("sound", p->name, ""));
		  break;

	       case param_hex:
		  if (p->value[0])
		     i = strtol(p->value, NULL, 16);
		  else
		     i = -1;
		  set_config_hex("sound", p->name, i);
		  strcpy(p->value, get_config_string("sound", p->name, ""));
		  break;

	       case param_bool:
		  set_config_int("sound", p->name, (d->flags & D_SELECTED) ? 1 : 0);
		  strcpy(p->value, get_config_string("sound", p->name, ""));
		  break;

	       case param_file:
		  set_config_string("sound", p->name, p->value);
		  strcpy(p->value, get_config_string("sound", p->name, ""));
		  break;

	       case param_list:
		  i = strtol(freq_getter(d->d1, NULL), NULL, 0);
		  set_config_int("sound", p->name, i);
		  strcpy(p->value, get_config_string("sound", p->name, ""));
		  break;

	       default:
		  break;
	    }

	 }

	 d++;
      }
   }
   else {
      /* discard the changes */ 
      while (d->proc) {
	 p = d->dp2;

	 if (p)
	    strcpy(p->value, get_config_string("sound", p->name, ""));

	 d++;
      }
   }

   return state_exit;
}

/* sets up the soundcard parameter dialog box */
void setup_param_dialog()
{
   PARAMETER *p;
   DIALOG *d = param_dlg;
   char **c = soundcard->param;
   char *s;
   int pos = 0;
   int xo = 0;
   int yo = 0;
   int i, x, y, f, g;

   #define DLG(_p, _x, _y, _w, _h, _f, _b, _k, _l, _d1, _d2, _dp, _pa, _m) \
   {                                                                       \
      d->proc = _p;                                                        \
      d->x = _x;                                                           \
      d->y = _y;                                                           \
      d->w = _w;                                                           \
      d->h = _h;                                                           \
      d->fg = _f;                                                          \
      d->bg = _b;                                                          \
      d->key = _k;                                                         \
      d->flags = _l;                                                       \
      d->d1 = _d1;                                                         \
      d->d2 = _d2;                                                         \
      d->dp = _dp;                                                         \
      d->dp2 = _pa;                                                        \
      d->dp3 = _m;                                                         \
      d++;                                                                 \
   }

   while (*c) {
      if ((isdigit((*c)[0])) && (isdigit((*c)[1]))) {
	 xo = (*c)[0] - '0';
	 if (xo)
	    xo = 100 / xo;

	 yo = (*c)[1] - '0';
	 if (yo)
	    yo = 38 / yo;
      }
      else {
	 x = 16 + (pos%3) * 100 + xo;
	 y = 30 + (pos/3) * 38 + yo;
	 pos++;

	 p = NULL;

	 for (i=0; parameters[i].name; i++) {
	    if (stricmp(parameters[i].name, *c) == 0) {
	       p = &parameters[i];
	       break;
	    }
	 }

	 if (p) {
	    switch (p->type) {

	       case param_int:
		  /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)  (dp)           (p)         (help) */
		  DLG(d_box_proc,      x,    y,    88,	 21,   255,  0,   0,	0,	 0,    0,    NULL,	    NULL,	NULL);
		  DLG(d_text_proc,     x+4,  y+3,  0,	 0,    255,  0,   0,	0,	 0,    0,    p->label,	    NULL,	NULL);
		  DLG(d_edit_proc,     x+54, y+3,  32,	 16,   255,  0,   0,	0,	 3,    0,    p->value,	    p,		p->desc);
		  break;

	       case param_hex:
		  if (stricmp(p->value, "FFFFFFFF") == 0)
		     strcpy(p->value, "-1");

		  /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)  (dp)           (p)         (help) */
		  DLG(d_box_proc,      x,    y,    88,	 21,   255,  0,   0,	0,	 0,    0,    NULL,	    NULL,	NULL);
		  DLG(d_text_proc,     x+4,  y+3,  0,	 0,    255,  0,   0,	0,	 0,    0,    p->label,	    NULL,	NULL);
		  DLG(d_edit_proc,     x+54, y+3,  32,	 16,   255,  0,   0,	0,	 3,    0,    p->value,	    p,		p->desc);
		  break;

	       case param_bool:
		  if (strtol(p->value, NULL, 0) != 0)
		     f = D_SELECTED;
		  else
		     f = 0;

		  /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)  (dp)           (p)         (help) */
		  DLG(d_box_proc,      x,    y,    88,	 21,   255,  0,   0,	0,	 0,    0,    NULL,	    NULL,	NULL);
		  DLG(d_text_proc,     x+4,  y+3,  0,	 0,    255,  0,   0,	0,	 0,    0,    p->label,	    NULL,	NULL);
		  DLG(d_check_proc,    x+54, y+3,  31,	 15,   255,  0,   0,	f,	 0,    0,    " ",           p,          p->desc);
		  break;

	       case param_file:
		  if (p->value[0])
		     f = D_SELECTED | D_EXIT;
		  else
		     f = D_EXIT;

		  /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)  (dp)           (p)         (help) */
		  DLG(d_box_proc,      x,    y,    88,	 21,   255,  0,   0,	0,	 0,    0,    NULL,	    NULL,	NULL);
		  DLG(d_text_proc,     x+4,  y+3,  0,	 0,    255,  0,   0,	0,	 0,    0,    p->label,	    NULL,	NULL);
		  DLG(filename_proc,   x+62, y+3,  31,	 15,   255,  0,   0,	f,	 0,    0,    "",            p,          p->desc);
		  break;

	       case param_list:
		  i = strtol(p->value, NULL, 0);
		  freq_getter(-1, &f);
		  if (i > 0) {
		     for (g=0; g<f; g++) {
			s = freq_getter(g, NULL);
			if (i <= strtol(s, NULL, 0))
			   break;
		     }
		  }
		  else
		     g = 2;

		  /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)  (dp)           (p)         (help) */
		  DLG(d_xlist_proc,    x,    y-8,  88,	 67,   255,  0,   0,	0,	 g,    0,    freq_getter,   p,		p->desc);
		  break;

	       default:
		  break;
	    }
	 }
      }

      c++;
   }

   param_ok = ((int)d - (int)param_dlg) / sizeof(DIALOG);

   /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)  (dp)        (p)                        (help) */
   DLG(d_button_proc,	30,   142,  124,  24,	255,   0,   13,   D_EXIT,  0,	 0,    "OK",       NULL,                      "Use these parameters");
   DLG(d_button_proc,	166,  142,  124,  24,	255,   0,   0,	  D_EXIT,  0,	 0,    "Cancel",   NULL,                      backup_str);
   DLG(scroller_proc,   0,    0,    0,    0,    0,    0,    0,    0,       0,    0,    NULL,       NULL,                      NULL);
   DLG(NULL,            0,    0,    0,    0,    0,    0,    0,    0,       0,    0,    NULL,       NULL,                      NULL);

   activate_dialog(param_dlg, param_handler, TRUE);
}

/* handle input from the test dialog */
DIALOG_STATE test_handler(int c)
{
   MIDI *mid;

   switch (c) {

      case 0:
	 /* MIDI test */
	 mid = load_midi("DATA/TUNE.MID");
	 if (mid != 0) play_midi(mid, FALSE);
	 return state_redraw;

      case 1:
	 /* left pan */
	 play_sample(test_sample, 255, 0, 1000, FALSE);
	 return state_redraw;

      case 2:
	 /* centre pan */
	 play_sample(test_sample, 255, 128, 1000, FALSE);
	 return state_redraw;

      case 3:
	 /* right pan */
	 play_sample(test_sample, 255, 255, 1000, FALSE);
	 return state_redraw;

      default:
	 /* quit */
	 remove_sound();
	 return state_exit;
   }

   return state_active;
}

/* handle input from the card selection dialog */
DIALOG_STATE card_handler(int c)
{
   int i;

   switch (c) {

      case 0:
      case 2:
	 /* select driver */
	 i = (soundcard == digi_cards) ? 0 : 1;
	 soundcard += card_dlg[2].d1;
	 set_config_int("sound", parameters[i].name, soundcard->id);
	 strcpy(parameters[i].value, get_config_string("sound", parameters[i].name, ""));
	 if (soundcard->param)
	    setup_param_dialog();
	 else
	    return state_exit;
	 break;

      default:
	 /* quit */
	 return state_exit;
   }

   return state_active;
}

/* handle input from the main dialog */
DIALOG_STATE main_handler(int c)
{
   char b1[80], b2[80];
   int i;

   switch (c) {

      case 0:
	 /* autodetect */
	 scroller_proc(MSG_RADIO, NULL, 0);

	 for (i=0; parameters[i].name; i++) {
	    set_config_string("sound", parameters[i].name, "");
	    parameters[i].value[0] = 0;
	 }

	 if (init_sound("Unable to autodetect!") != 0)
	    return state_redraw;

	 sprintf(b1, "Digital: %s", digi_driver->name);
	 sprintf(b2, "MIDI: %s", midi_driver->name);
	 alert("- detected hardware -", b1, b2, "Ok", NULL, 0, 0);

	 for (i=0; parameters[i].name; i++) {
	    if (parameters[i].detect) {
	       switch (parameters[i].type) {

		  case param_int:
		  case param_bool:
		  case param_list:
		     set_config_int("sound", parameters[i].name, *parameters[i].detect);
		     break;

		  case param_hex:
		     set_config_hex("sound", parameters[i].name, *parameters[i].detect);
		     break;

		  default:
		     break;
	       }
	    }
	    else
	       set_config_string("sound", parameters[i].name, parameters[i].def);

	    strcpy(parameters[i].value, get_config_string("sound", parameters[i].name, ""));
	 }

	 remove_sound();
	 return state_redraw;

      case 1:
	 /* test */
	 scroller_proc(MSG_RADIO, NULL, 0);
	 if (init_sound("Sound initialization failed!") != 0)
	    return state_redraw;
	 sprintf(midi_desc, "Driver: %s        Description: %s", midi_driver->name, midi_driver->desc);
	 sprintf(digi_desc, "Driver: %s        Description: %s", digi_driver->name, digi_driver->desc);
	 activate_dialog(test_dlg, test_handler, FALSE);
	 break;

      case 2:
	 /* choose digital driver */
	 soundcard = digi_cards;
	 for (i=0; soundcard[i].id; i++)
	    if (soundcard[i].id == get_config_int("sound", "digi_card", DIGI_AUTODETECT))
	       break;
	 card_dlg[2].d1 = i;
	 activate_dialog(card_dlg, card_handler, FALSE);
	 break;

      case 3:
	 /* choose MIDI driver */
	 soundcard = midi_cards;
	 for (i=0; soundcard[i].id; i++)
	    if (soundcard[i].id == get_config_int("sound", "midi_card", MIDI_AUTODETECT))
	       break;
	 card_dlg[2].d1 = i;
	 activate_dialog(card_dlg, card_handler, FALSE);
	 break;

      case 4:
	 /* save settings and quit */
	 set_config_file(CFG_FILE);
	 for (i=0; parameters[i].name; i++) {
	    if (parameters[i].value[0])
	       set_config_string("sound", parameters[i].name, parameters[i].value);
	    else
	       set_config_string("sound", parameters[i].name, " ");
	 }
	 return state_exit;

      default:
	 /* quit */
	 return state_exit;
   }

   return state_active;
}

void setsound(void)
{
   int i;
   
   allegro_init();
   install_mouse();
   install_keyboard();
   install_timer();

   data = load_datafile("DATA/SETSOUND.DAT");

   fade_out(4);
   set_gfx_mode(GFX_VGA, 320, 200, 0, 0);
   set_pallete(black_palette);

   set_mouse_range(0, 20, SCREEN_W-1, SCREEN_H-32);

   font = setup_font;

   memset(scroller_msg, ' ', sizeof(scroller_msg));

   buffer = create_bitmap(SCREEN_W, SCREEN_H);

   LOCK_VARIABLE(scroller_time);
   LOCK_FUNCTION(inc_scroller_time);
   install_int_ex(inc_scroller_time, BPS_TO_TIMER(160));

   set_config_file(CFG_FILE);
   for (i=0; parameters[i].name; i++) {
      strcpy(parameters[i].value, get_config_string("sound", parameters[i].name, parameters[i].def));
      if (!parameters[i].value[0])
	 strcpy(parameters[i].value, parameters[i].def);
   }

   set_config_data("", 0);
   for (i=0; parameters[i].name; i++)
      set_config_string("sound", parameters[i].name, parameters[i].value);

   detect_sound();

   activate_dialog(main_dlg, main_handler, FALSE);
   dialog_count++;

   do {
   } while (update());

   destroy_bitmap(buffer);

   set_mouse_range(0, 0, SCREEN_W-1, SCREEN_H-1);

   exit(0);
}
