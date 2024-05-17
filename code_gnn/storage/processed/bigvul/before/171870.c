void btif_hl_free_buf(void **p)
{
 if (*p != NULL)
 {
        BTIF_TRACE_DEBUG("%s OK", __FUNCTION__ );
        GKI_freebuf(*p);
 *p = NULL;
 }
 else
        BTIF_TRACE_ERROR("%s NULL pointer",__FUNCTION__ );
}
