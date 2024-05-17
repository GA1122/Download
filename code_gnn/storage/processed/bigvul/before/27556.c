static void enter_4op_mode(void)
{
	int i;
	static int v4op[MAX_VOICE] = {
		0, 1, 2, 9, 10, 11, 6, 7, 8, 15, 16, 17
	};

	devc->cmask = 0x3f;	 
	opl3_command(devc->right_io, CONNECTION_SELECT_REGISTER, 0x3f);

	for (i = 0; i < 3; i++)
		pv_map[i].voice_mode = 4;
	for (i = 3; i < 6; i++)
		pv_map[i].voice_mode = 0;

	for (i = 9; i < 12; i++)
		pv_map[i].voice_mode = 4;
	for (i = 12; i < 15; i++)
		pv_map[i].voice_mode = 0;

	for (i = 0; i < 12; i++)
		devc->lv_map[i] = v4op[i];
	devc->v_alloc->max_voice = devc->nr_voice = 12;
}