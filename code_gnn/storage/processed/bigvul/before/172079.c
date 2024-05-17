static void free_thread_slot(int h)
{
 if(0 <= h && h < MAX_THREAD)
 {
        close_cmd_fd(h);
        ts[h].used = 0;
 }
 else APPL_TRACE_ERROR("invalid thread handle:%d", h);
}
