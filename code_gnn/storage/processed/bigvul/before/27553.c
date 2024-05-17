static void bend_pitch(int dev, int voice, int value)
{
	unsigned char data;
	int block, fnum, freq;
	struct physical_voice_info *map;

	map = &pv_map[devc->lv_map[voice]];

	if (map->voice_mode == 0)
		return;

	devc->voc[voice].bender = value;
	if (!value)
		return;
	if (!(devc->voc[voice].keyon_byte & 0x20))
		return;	 

	freq = compute_finetune(devc->voc[voice].orig_freq, devc->voc[voice].bender, devc->voc[voice].bender_range, 0);
	devc->voc[voice].current_freq = freq;

	freq_to_fnum(freq, &block, &fnum);

	data = fnum & 0xff;	 
	opl3_command(map->ioaddr, FNUM_LOW + map->voice_num, data);

	data = 0x20 | ((block & 0x7) << 2) | ((fnum >> 8) & 0x3);
	devc->voc[voice].keyon_byte = data;
	opl3_command(map->ioaddr, KEYON_BLOCK + map->voice_num, data);
}