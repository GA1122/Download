int flush_old_exec(struct linux_binprm * bprm)
{
	int retval;

	 
	retval = de_thread(current);
	if (retval)
		goto out;

	 
	set_mm_exe_file(bprm->mm, bprm->file);

	 
	acct_arg_size(bprm, 0);
	retval = exec_mmap(bprm->mm);
	if (retval)
		goto out;

	bprm->mm = NULL;		 

	set_fs(USER_DS);
	current->flags &= ~(PF_RANDOMIZE | PF_FORKNOEXEC | PF_KTHREAD |
					PF_NOFREEZE | PF_NO_SETAFFINITY);
	flush_thread();
	current->personality &= ~bprm->per_clear;

	return 0;

out:
	return retval;
}
