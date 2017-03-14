/* Functions to load/commence playing music/samples. Sounds are created
   either at a fixed location or relative to an object. In each case the
   distance to the players tank is used to compute the volume. In stereo
   mode, player 1 hears through the left channel and player 2 through
   the right channel.
*/

#include <tank.h>
#include <math.h>
#include <string.h>

long sound_volume = 255, music_volume = 255;

static DATAFILE *wav;

void init_player(char *tune)
{
  static MIDI *mf = 0;
  static char *tn = "";

  if (midi_driver == &midi_none)
    return;

  if (stricmp(tn, tune) == 0)
    return;

  if (mf != 0)
  {
    stop_midi(mf);
    destroy_midi(mf);
  }

  if ((mf = load_midi(tune)) != 0)
  {
    play_midi(mf, TRUE);
    tn = tune;
  }
}

void init_sound_card(void)
{
  if (!install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL))
    printf("Sound: %s\n"
           "Midi: %s\n\n", digi_driver->desc, midi_driver->desc);
  else
    printf("Sound initialisation failed!\n\n");

  set_volume(0, music_volume);
}

void init_sounds(void)
{ 
  if (digi_driver == &digi_none)
    return;

  wav = load_datafile("DATA/SOUND.DAT");

  if (wav == 0)
    error("Unable to open DATA/SOUND.DAT!\n");
}

void create_sound_absolute(long x, long y, long sample)
{
  long vol, pan = 128, d = 612, d1, d2;

  if (digi_driver == &digi_none || !game.running)
    return;

  if (wav[sample].dat != NULL)
  {
    switch(game.mode)
    {
      case SINGLE:
       if (game.tk[0]!=NULL)
	 d = distance(x-game.tk[0]->x, y-game.tk[0]->y);
       break;

      case DUAL:
      case DUALVC:
	if (game.tk[0]!=NULL)
	{
	  d1 = distance(x-game.tk[0]->x, y-game.tk[0]->y);
	  if (d1 > 612) d1 = 612;
	} else d1 = 612;

       if (game.tk[1]!=NULL)
       {
	 d2 = distance(x-game.tk[1]->x, y-game.tk[1]->y);
	 if (d2 > 612) d2 = 612;
       } else d2 = 612;

       d = min(d1, d2);
       pan = -(612-d1)/4+(612-d2)/4+128;
       if (pan<0) pan=0;
       if (pan>255) pan=255;
       break;

      case MAP:
       break;
    }

    if (d < 100) vol = 255;
    else if (d < 612) vol = (612-d)/2;
    else vol = 0;

    if (vol > 32)
      play_sample(wav[sample].dat, vol, pan, 900+rnd(200), FALSE);
  }
}
 
