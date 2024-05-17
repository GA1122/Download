void * btif_hl_get_buf(UINT16 size)
{
 void *p_new;

    BTIF_TRACE_DEBUG("%s", __FUNCTION__);
    BTIF_TRACE_DEBUG("ret size=%d GKI_MAX_BUF_SIZE=%d",size, 6000);

 if (size < 6000)
 {
        p_new = GKI_getbuf(size);
 }
 else
 {
        BTIF_TRACE_DEBUG("btif_hl_get_buf use HL large data pool");
        p_new = GKI_getpoolbuf(4);
 }

 return p_new;
}
