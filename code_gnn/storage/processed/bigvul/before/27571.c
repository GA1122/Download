static int opl3_set_instr  (int dev, int voice, int instr_no)
{
	if (voice < 0 || voice >= devc->nr_voice)
		return 0;
	if (instr_no < 0 || instr_no >= SBFM_MAXINSTR)
		instr_no = 0;	 

	devc->act_i[voice] = &devc->i_map[instr_no];
	return 0;
}