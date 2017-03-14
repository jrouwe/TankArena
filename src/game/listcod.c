/* Cheat: List all codes from maps and if the map is locked or not */

#include <tank.h>
#include <dir.h>
#include <string.h>
#include <unistd.h>

void search_passwd()
{
  struct ffblk ffblk;
  int done;

  done=findfirst("MAPS/*.MAP", &ffblk, 0);

  while(!done)
  {
    fclose(read_header(ffblk.ff_name, "", "rb", &map));

    if (map.map_type == MAPT_DUALVC || map.public_pwd)
      add_password_to_list(map.this_mission_code);

    done=findnext(&ffblk);
  }
}

void list_codes(void)
{ long done;
  struct ffblk ffblk;
  int count = 0;

  done=findfirst("MAPS/*.MAP", &ffblk, 0);

  printf("L = Locked, P = Public password, S = Shareware, T = Type\n"
	 "Type: 0 = Any, 1 = Dual, 2 = Single, 3 = Dualvc, 4 = S/D\n\n");

  printf("%1s %1s %1s %1s %-15s %-22s %-22s\n", "L", "P", "S", "T", "File", "Mission code", "Next code");
  printf("%1s %1s %1s %1s %-15s %-22s %-22s\n", "-", "-", "-", "-", "----", "------------", "---------");

  while(!done)
  { fclose(read_header(ffblk.ff_name, "", "rb", &map));

    if (map.map_type != MAPT_DUAL)
      printf("%1s %1s %1s %1d %-15s %-22s %-22s\n", map.lock? "Y":"N", map.public_pwd? "Y":"N", map.shareware? "Y":"N", map.map_type, ffblk.ff_name, map.this_mission_code, map.next_mission_code);
    else
      printf("%1s - %1s %1d %-15s\n", map.lock? "Y":"N", map.shareware? "Y":"N", map.map_type, ffblk.ff_name);

    count++;

    done=findnext(&ffblk);
  }

  printf("\nTotal: %d\n", count);

  exit(0);
}
