static int cmd_bsize(void *data, const char *input) {
	ut64 n;
	RFlagItem *flag;
	RCore *core = (RCore *)data;
	switch (input[0]) {
	case 'm':
		n = r_num_math (core->num, input + 1);
		if (n > 1) core->blocksize_max = n;
		else r_cons_printf ("0x%x\n", (ut32)core->blocksize_max);
		break;
	case '+':
		n = r_num_math (core->num, input + 1);
		r_core_block_size (core, core->blocksize + n);
		break;
	case '-':
		n = r_num_math (core->num, input + 1);
		r_core_block_size (core, core->blocksize - n);
		break;
	case 'f':
		if (input[1] == ' ') {
			flag = r_flag_get (core->flags, input + 2);
			if (flag) {
				r_core_block_size (core, flag->size);
			} else {
				eprintf ("bf: cannot find flag named '%s'\n", input + 2);
			}
		} else {
			eprintf ("Usage: bf [flagname]\n");
		}
		break;
	case '\0':
		r_cons_printf ("0x%x\n", core->blocksize);
		break;
	case '?':{
		const char* help_msg[] = {
			"Usage:",  "b[f] [arg]\n", "Get/Set block size",
			"b", "", "display current block size",
			"b", " 33", "set block size to 33",
			"b", "+3", "increase blocksize by 3",
			"b", "-16", "decrease blocksize by 16",
			"b", " eip+4", "numeric argument can be an expression",
			"bf", " foo", "set block size to flag size",
			"bm", " 1M", "set max block size",
			NULL
		};
		r_core_cmd_help (core, help_msg);
		}
		break;
	default:
		r_core_block_size (core, r_num_math (core->num, input));
		break;
	}
	return 0;
}