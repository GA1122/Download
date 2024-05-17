static int opl3_alloc_voice(int dev, int chn, int note, struct voice_alloc_info *alloc)
{
	int i, p, best, first, avail, best_time = 0x7fffffff;
	struct sbi_instrument *instr;
	int is4op;
	int instr_no;

	if (chn < 0 || chn > 15)
		instr_no = 0;
	else
		instr_no = devc->chn_info[chn].pgm_num;

	instr = &devc->i_map[instr_no];
	if (instr->channel < 0 ||	 
		devc->nr_voice != 12)	 
		is4op = 0;
	else if (devc->nr_voice == 12)	 
		is4op = (instr->key == OPL3_PATCH);
	else
		is4op = 0;

	if (is4op)
	{
		first = p = 0;
		avail = 6;
	}
	else
	{
		if (devc->nr_voice == 12)	 
			first = p = 6;
		else
			first = p = 0;
		avail = devc->nr_voice;
	}

	 
	best = first;

	for (i = 0; i < avail; i++)
	{
		if (alloc->map[p] == 0)
		{
			return p;
		}
		if (alloc->alloc_times[p] < best_time)		 
		{
			best_time = alloc->alloc_times[p];
			best = p;
		}
		p = (p + 1) % avail;
	}

	 

	if (best < 0)
		best = 0;
	if (best > devc->nr_voice)
		best -= devc->nr_voice;

	return best;	 
}
