local void copymeta(char *from, char *to)
{
    struct stat st;
    struct timeval times[2];

     
    if (stat(from, &st) != 0 || (st.st_mode & S_IFMT) != S_IFREG)
        return;

     
    (void)chmod(to, st.st_mode & 07777);

     
    (void)chown(to, st.st_uid, st.st_gid);

     
    times[0].tv_sec = st.st_atime;
    times[0].tv_usec = 0;
    times[1].tv_sec = st.st_mtime;
    times[1].tv_usec = 0;
    (void)utimes(to, times);
}
