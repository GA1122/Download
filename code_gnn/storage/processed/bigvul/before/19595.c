static int reverse_path_check(void)
{
	int error = 0;
	struct file *current_file;

	 
	list_for_each_entry(current_file, &tfile_check_list, f_tfile_llink) {
		path_count_init();
		error = ep_call_nested(&poll_loop_ncalls, EP_MAX_NESTS,
					reverse_path_check_proc, current_file,
					current_file, current);
		if (error)
			break;
	}
	return error;
}