int btsock_thread_init()
{
 static int initialized;
    APPL_TRACE_DEBUG("in initialized:%d", initialized);
 if(!initialized)
 {
        initialized = 1;
 int h;
 for(h = 0; h < MAX_THREAD; h++)
 {
            ts[h].cmd_fdr = ts[h].cmd_fdw = -1;
            ts[h].used = 0;
            ts[h].thread_id = -1;
            ts[h].poll_count = 0;
            ts[h].callback = NULL;
            ts[h].cmd_callback = NULL;
 }
 }
 return TRUE;
}
