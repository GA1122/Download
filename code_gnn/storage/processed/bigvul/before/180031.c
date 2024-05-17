 int open_debug_log(void) {
// int open_debug_log(void)
// {
// 	int fh;
// 	struct stat st;
  
  	 
  	if(verify_config || test_scheduling == TRUE)
 		return OK;
 
 	 
  	if(debug_level == DEBUGL_NONE)
  		return OK;
  
	if((debug_file_fp = fopen(debug_file, "a+")) == NULL)
// 	if ((fh = open(debug_file, O_RDWR|O_APPEND|O_CREAT|O_NOFOLLOW, S_IRUSR|S_IWUSR)) == -1)
// 		return ERROR;
// 	if((debug_file_fp = fdopen(fh, "a+")) == NULL)
// 		return ERROR;
// 
// 	if ((fstat(fh, &st)) == -1) {
// 		debug_file_fp = NULL;
// 		close(fh);
// 		return ERROR;
// 	}
// 	if (st.st_nlink != 1 || (st.st_mode & S_IFMT) != S_IFREG) {
// 		debug_file_fp = NULL;
// 		close(fh);
  		return ERROR;
// 	}
  
	(void)fcntl(fileno(debug_file_fp), F_SETFD, FD_CLOEXEC);
// 	(void)fcntl(fh, F_SETFD, FD_CLOEXEC);
  
  	return OK;
  	}