static inline void close_cmd_fd(int h)
{
 if(ts[h].cmd_fdr != -1)
 {
        close(ts[h].cmd_fdr);
        ts[h].cmd_fdr = -1;
 }
 if(ts[h].cmd_fdw != -1)
 {
        close(ts[h].cmd_fdw);
        ts[h].cmd_fdw = -1;
 }
}
