static void scan_syslog_file(GList **oops_list, int fd)
{
    struct stat st;
    struct stat *statbuf = &st;

     
    int sz = MAX_SCAN_BLOCK - READ_AHEAD;
     
    off_t cur_pos = lseek(fd, 0, SEEK_CUR);
    if (cur_pos >= 0 && fstat(fd, statbuf) == 0 && S_ISREG(statbuf->st_mode))
    {
        off_t size_to_read = statbuf->st_size - cur_pos;
        if (size_to_read >= 0 && sz > size_to_read)
            sz = size_to_read;
    }

     
    sz += READ_AHEAD;
    char *buffer = xzalloc(sz);

    for (;;)
    {
        int r = full_read(fd, buffer, sz-1);
        if (r <= 0)
            break;
        log_debug("Read %u bytes", r);
        koops_extract_oopses(oops_list, buffer, r);
    }

    free(buffer);
}
