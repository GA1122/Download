static int alloc_thread_slot()
{
 int i;
 for(i = MAX_THREAD - 1; i >=0; i--)
 {
        APPL_TRACE_DEBUG("ts[%d].used:%d", i, ts[i].used);
 if(!ts[i].used)
 {
            ts[i].used = 1;
 return i;
 }
 }
    APPL_TRACE_ERROR("execeeded max thread count");
 return -1;
}
