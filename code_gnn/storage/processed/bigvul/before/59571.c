static int compareFiles(const char *r1  , const char *r2  ) {
    int res1, res2;
    int fd1, fd2;
    char bytes1[4096];
    char bytes2[4096];

    if (update_results) {
        fd1 = open(r1, RD_FLAGS);
        if (fd1 < 0)
            return(-1);
        fd2 = open(r2, WR_FLAGS, 0644);
        if (fd2 < 0) {
            close(fd1);
            return(-1);
        }
        do {
            res1 = read(fd1, bytes1, 4096);
            if (res1 <= 0)
                break;
            res2 = write(fd2, bytes1, res1);
            if (res2 <= 0 || res2 != res1)
                break;
        } while (1);
        close(fd2);
        close(fd1);
        return(res1 != 0);
    }

    fd1 = open(r1, RD_FLAGS);
    if (fd1 < 0)
        return(-1);
    fd2 = open(r2, RD_FLAGS);
    if (fd2 < 0) {
        close(fd1);
        return(-1);
    }
    while (1) {
        res1 = read(fd1, bytes1, 4096);
        res2 = read(fd2, bytes2, 4096);
	if ((res1 != res2) || (res1 < 0)) {
	    close(fd1);
	    close(fd2);
	    return(1);
	}
	if (res1 == 0)
	    break;
	if (memcmp(bytes1, bytes2, res1) != 0) {
	    close(fd1);
	    close(fd2);
	    return(1);
	}
    }
    close(fd1);
    close(fd2);
    return(0);
}