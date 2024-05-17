static void mode_to_access_flags(umode_t mode, umode_t bits_to_use,
				__u32 *pace_flags)
{
	 
	*pace_flags = 0x0;

	 
	mode &= bits_to_use;

	 
	if (mode & S_IRUGO)
		*pace_flags |= SET_FILE_READ_RIGHTS;
	if (mode & S_IWUGO)
		*pace_flags |= SET_FILE_WRITE_RIGHTS;
	if (mode & S_IXUGO)
		*pace_flags |= SET_FILE_EXEC_RIGHTS;

	cifs_dbg(NOISY, "mode: 0x%x, access flags now 0x%x\n",
		 mode, *pace_flags);
	return;
}
