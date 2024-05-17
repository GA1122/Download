set_pwnam_buf_len(void)
{
	long buf_len;

	 
	if ((buf_len = sysconf(_SC_GETPW_R_SIZE_MAX)) == -1)
		getpwnam_buf_len = 1024;	 
	else
		getpwnam_buf_len = (size_t)buf_len;
	if ((buf_len = sysconf(_SC_GETGR_R_SIZE_MAX)) != -1 &&
	    (size_t)buf_len > getpwnam_buf_len)
		getpwnam_buf_len = (size_t)buf_len;
}
