#include<iostream>
#include<ctime>
#include<cerrno>
#include<unistd.h>
#include<vlc/vlc.h>

using namespace std;
 
 void vlc(string path, int sleepval)
 {
     libvlc_instance_t * inst;
     libvlc_media_player_t *mp;
     libvlc_media_t *m;
     
     inst = libvlc_new (0, NULL);
     m = libvlc_media_new_path (inst, path);
     mp = libvlc_media_player_new_from_media (m);
     libvlc_media_release (m);
     libvlc_media_player_play (mp);
     sleep(10);
     libvlc_media_player_stop (mp);
     libvlc_media_player_release (mp);
     libvlc_release (inst);
 }

