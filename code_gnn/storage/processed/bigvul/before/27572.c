static int opl3_start_note (int dev, int voice, int note, int volume)
{
	unsigned char data, fpc;
	int block, fnum, freq, voice_mode, pan;
	struct sbi_instrument *instr;
	struct physical_voice_info *map;

	if (voice < 0 || voice >= devc->nr_voice)
		return 0;

	map = &pv_map[devc->lv_map[voice]];
	pan = devc->voc[voice].panning;

	if (map->voice_mode == 0)
		return 0;

	if (note == 255)	 
	{
		set_voice_volume(voice, volume, devc->voc[voice].volume);
		return 0;
	}

	 
	
	opl3_command(map->ioaddr, KSL_LEVEL + map->op[1], 0xff);	 
	opl3_command(map->ioaddr, KSL_LEVEL + map->op[0], 0xff);	 

	if (map->voice_mode == 4)
	{
		opl3_command(map->ioaddr, KSL_LEVEL + map->op[2], 0xff);
		opl3_command(map->ioaddr, KSL_LEVEL + map->op[3], 0xff);
	}

	opl3_command(map->ioaddr, KEYON_BLOCK + map->voice_num, 0x00);	 

	instr = devc->act_i[voice];
	
	if (!instr)
		instr = &devc->i_map[0];

	if (instr->channel < 0)
	{
		printk(KERN_WARNING "opl3: Initializing voice %d with undefined instrument\n", voice);
		return 0;
	}

	if (map->voice_mode == 2 && instr->key == OPL3_PATCH)
		return 0;	 

	voice_mode = map->voice_mode;

	if (voice_mode == 4)
	{
		int voice_shift;

		voice_shift = (map->ioaddr == devc->left_io) ? 0 : 3;
		voice_shift += map->voice_num;

		if (instr->key != OPL3_PATCH)	 
		{
			voice_mode = 2;
			devc->cmask &= ~(1 << voice_shift);
		}
		else
		{
			devc->cmask |= (1 << voice_shift);
		}

		opl3_command(devc->right_io, CONNECTION_SELECT_REGISTER, devc->cmask);
	}

	 
	
	opl3_command(map->ioaddr, AM_VIB + map->op[0], instr->operators[0]);
	opl3_command(map->ioaddr, AM_VIB + map->op[1], instr->operators[1]);

	 
	
	opl3_command(map->ioaddr, ATTACK_DECAY + map->op[0], instr->operators[4]);
	opl3_command(map->ioaddr, ATTACK_DECAY + map->op[1], instr->operators[5]);

	 
	
	opl3_command(map->ioaddr, SUSTAIN_RELEASE + map->op[0], instr->operators[6]);
	opl3_command(map->ioaddr, SUSTAIN_RELEASE + map->op[1], instr->operators[7]);

	 

	opl3_command(map->ioaddr, WAVE_SELECT + map->op[0], instr->operators[8]);
	opl3_command(map->ioaddr, WAVE_SELECT + map->op[1], instr->operators[9]);

	 
	
	fpc = instr->operators[10];

	if (pan != 0xffff)
	{
		fpc &= ~STEREO_BITS;
		if (pan < -64)
			fpc |= VOICE_TO_LEFT;
		else
			if (pan > 64)
				fpc |= VOICE_TO_RIGHT;
			else
				fpc |= (VOICE_TO_LEFT | VOICE_TO_RIGHT);
	}

	if (!(fpc & 0x30))
		fpc |= 0x30;	 
	opl3_command(map->ioaddr, FEEDBACK_CONNECTION + map->voice_num, fpc);

	 

	if (voice_mode == 4)
	{
		 
	
		opl3_command(map->ioaddr, AM_VIB + map->op[2], instr->operators[OFFS_4OP + 0]);
		opl3_command(map->ioaddr, AM_VIB + map->op[3], instr->operators[OFFS_4OP + 1]);

		 
		
		opl3_command(map->ioaddr, ATTACK_DECAY + map->op[2], instr->operators[OFFS_4OP + 4]);
		opl3_command(map->ioaddr, ATTACK_DECAY + map->op[3], instr->operators[OFFS_4OP + 5]);

		 
		
		opl3_command(map->ioaddr, SUSTAIN_RELEASE + map->op[2], instr->operators[OFFS_4OP + 6]);
		opl3_command(map->ioaddr, SUSTAIN_RELEASE + map->op[3], instr->operators[OFFS_4OP + 7]);

		 
		
		opl3_command(map->ioaddr, WAVE_SELECT + map->op[2], instr->operators[OFFS_4OP + 8]);
		opl3_command(map->ioaddr, WAVE_SELECT + map->op[3], instr->operators[OFFS_4OP + 9]);

		 
		
		fpc = instr->operators[OFFS_4OP + 10];
		if (!(fpc & 0x30))
			 fpc |= 0x30;	 
		opl3_command(map->ioaddr, FEEDBACK_CONNECTION + map->voice_num + 3, fpc);
	}

	devc->voc[voice].mode = voice_mode;
	set_voice_volume(voice, volume, devc->voc[voice].volume);

	freq = devc->voc[voice].orig_freq = note_to_freq(note) / 1000;

	 

	freq = compute_finetune(devc->voc[voice].orig_freq, devc->voc[voice].bender, devc->voc[voice].bender_range, 0);
	devc->voc[voice].current_freq = freq;

	freq_to_fnum(freq, &block, &fnum);

	 

	data = fnum & 0xff;	 
	opl3_command(map->ioaddr, FNUM_LOW + map->voice_num, data);

	data = 0x20 | ((block & 0x7) << 2) | ((fnum >> 8) & 0x3);
		 devc->voc[voice].keyon_byte = data;
	opl3_command(map->ioaddr, KEYON_BLOCK + map->voice_num, data);
	if (voice_mode == 4)
		opl3_command(map->ioaddr, KEYON_BLOCK + map->voice_num + 3, data);

	return 0;
}