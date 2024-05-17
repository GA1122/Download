int print_filename(char *pathname, struct inode *inode)
{
	char str[11], dummy[12], dummy2[12];  
	char *userstr, *groupstr;
	int padchars;
	struct passwd *user;
	struct group *group;
	struct tm *t;

	if(short_ls) {
		printf("%s\n", pathname);
		return 1;
	}

	user = getpwuid(inode->uid);
	if(user == NULL) {
		int res = snprintf(dummy, 12, "%d", inode->uid);
		if(res < 0)
			EXIT_UNSQUASH("snprintf failed in print_filename()\n");
		else if(res >= 12)
			 
			userstr = "*";
		else
			userstr = dummy;
	} else
		userstr = user->pw_name;
		 
	group = getgrgid(inode->gid);
	if(group == NULL) {
		int res = snprintf(dummy2, 12, "%d", inode->gid);
		if(res < 0)
			EXIT_UNSQUASH("snprintf failed in print_filename()\n");
		else if(res >= 12)
			 
			groupstr = "*";
		else
			groupstr = dummy2;
	} else
		groupstr = group->gr_name;

	printf("%s %s/%s ", modestr(str, inode->mode), userstr, groupstr);

	switch(inode->mode & S_IFMT) {
		case S_IFREG:
		case S_IFDIR:
		case S_IFSOCK:
		case S_IFIFO:
		case S_IFLNK:
			padchars = TOTALCHARS - strlen(userstr) -
				strlen(groupstr);

			printf("%*lld ", padchars > 0 ? padchars : 0,
				inode->data);
			break;
		case S_IFCHR:
		case S_IFBLK:
			padchars = TOTALCHARS - strlen(userstr) -
				strlen(groupstr) - 7; 

			printf("%*s%3d,%3d ", padchars > 0 ? padchars : 0, " ",
				(int) inode->data >> 8, (int) inode->data &
				0xff);
			break;
	}

	t = localtime(&inode->time);

	printf("%d-%02d-%02d %02d:%02d %s", t->tm_year + 1900, t->tm_mon + 1,
		t->tm_mday, t->tm_hour, t->tm_min, pathname);
	if((inode->mode & S_IFMT) == S_IFLNK)
		printf(" -> %s", inode->symlink);
	printf("\n");
		
	return 1;
}