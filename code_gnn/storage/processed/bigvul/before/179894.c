 int mount_proc_if_needed(const char *rootfs)
 {
 	char path[MAXPATHLEN];
 	char link[20];
 	int linklen, ret;
 	int mypid;
 
 	ret = snprintf(path, MAXPATHLEN, "%s/proc/self", rootfs);
 	if (ret < 0 || ret >= MAXPATHLEN) {
 		SYSERROR("proc path name too long");
 		return -1;
 	}
 	memset(link, 0, 20);
 	linklen = readlink(path, link, 20);
 	mypid = (int)getpid();
 	INFO("I am %d, /proc/self points to '%s'", mypid, link);
 	ret = snprintf(path, MAXPATHLEN, "%s/proc", rootfs);
 	if (ret < 0 || ret >= MAXPATHLEN) {
 		SYSERROR("proc path name too long");
 		return -1;
 	}
 	if (linklen < 0)  
 		goto domount;
 	if (atoi(link) != mypid) {
 		 
 		umount2(path, MNT_DETACH);  
 		goto domount;
 	}
 	 
  	return 0;
  
  domount:
	if (mount("proc", path, "proc", 0, NULL))
// 	if (safe_mount("proc", path, "proc", 0, NULL, rootfs) < 0)
  		return -1;
  	INFO("Mounted /proc in container for security transition");
  	return 1;
 }