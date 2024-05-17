static int opl3_kill_note  (int devno, int voice, int note, int velocity)
{
	 struct physical_voice_info *map;

	 if (voice < 0 || voice >= devc->nr_voice)
		 return 0;

	 devc->v_alloc->map[voice] = 0;

	 map = &pv_map[devc->lv_map[voice]];
	 DEB(printk("Kill note %d\n", voice));

	 if (map->voice_mode == 0)
		 return 0;

	 opl3_command(map->ioaddr, KEYON_BLOCK + map->voice_num, devc->voc[voice].keyon_byte & ~0x20);
	 devc->voc[voice].keyon_byte = 0;
	 devc->voc[voice].bender = 0;
	 devc->voc[voice].volume = 64;
	 devc->voc[voice].panning = 0xffff;	 
	 devc->voc[voice].bender_range = 200;
	 devc->voc[voice].orig_freq = 0;
	 devc->voc[voice].current_freq = 0;
	 devc->voc[voice].mode = 0;
	 return 0;
}