file_or_fd(struct magic_set *ms, const char *inname, int fd)
{
	int	rv = -1;
	unsigned char *buf;
	struct stat	sb;
	ssize_t nbytes = 0;	 
	int	ispipe = 0;
	off_t	pos = (off_t)-1;

	if (file_reset(ms) == -1)
		goto out;

	 
#define SLOP (1 + sizeof(union VALUETYPE))
	if ((buf = CAST(unsigned char *, malloc(HOWMANY + SLOP))) == NULL)
		return NULL;

	switch (file_fsmagic(ms, inname, &sb)) {
	case -1:		 
		goto done;
	case 0:			 
		break;
	default:		 
		rv = 0;
		goto done;
	}

#ifdef WIN32
	 
	if (fd == STDIN_FILENO)
		_setmode(STDIN_FILENO, O_BINARY);
#endif

	if (inname == NULL) {
		if (fstat(fd, &sb) == 0 && S_ISFIFO(sb.st_mode))
			ispipe = 1;
		else
			pos = lseek(fd, (off_t)0, SEEK_CUR);
	} else {
		int flags = O_RDONLY|O_BINARY;
		int okstat = stat(inname, &sb) == 0;

		if (okstat && S_ISFIFO(sb.st_mode)) {
#ifdef O_NONBLOCK
			flags |= O_NONBLOCK;
#endif
			ispipe = 1;
		}

		errno = 0;
		if ((fd = open(inname, flags)) < 0) {
#ifdef WIN32
			 
			if (!okstat && errno == EACCES) {
				sb.st_mode = S_IFBLK;
				okstat = 1;
			}
#endif
			if (okstat &&
			    unreadable_info(ms, sb.st_mode, inname) == -1)
				goto done;
			rv = 0;
			goto done;
		}
#ifdef O_NONBLOCK
		if ((flags = fcntl(fd, F_GETFL)) != -1) {
			flags &= ~O_NONBLOCK;
			(void)fcntl(fd, F_SETFL, flags);
		}
#endif
	}

	 
	if (ispipe) {
		ssize_t r = 0;

		while ((r = sread(fd, (void *)&buf[nbytes],
		    (size_t)(HOWMANY - nbytes), 1)) > 0) {
			nbytes += r;
			if (r < PIPE_BUF) break;
		}

		if (nbytes == 0) {
			 
			if (unreadable_info(ms, sb.st_mode, inname) == -1)
				goto done;
			rv = 0;
			goto done;
		}

	} else {
		 
		size_t howmany =
#if defined(WIN32) && HOWMANY > 8 * 1024
				_isatty(fd) ? 8 * 1024 :
#endif
				HOWMANY;
		if ((nbytes = read(fd, (char *)buf, howmany)) == -1) {
			if (inname == NULL && fd != STDIN_FILENO)
				file_error(ms, errno, "cannot read fd %d", fd);
			else
				file_error(ms, errno, "cannot read `%s'",
				    inname == NULL ? "/dev/stdin" : inname);
			goto done;
		}
	}

	(void)memset(buf + nbytes, 0, SLOP);  
	if (file_buffer(ms, fd, inname, buf, (size_t)nbytes) == -1)
		goto done;
	rv = 0;
done:
	free(buf);
	if (pos != (off_t)-1)
		(void)lseek(fd, pos, SEEK_SET);
	close_and_restore(ms, inname, fd, &sb);
out:
	return rv == 0 ? file_getbuffer(ms) : NULL;
}