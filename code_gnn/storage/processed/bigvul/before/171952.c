void btpan_close_handle(btpan_conn_t *p)
{
    BTIF_TRACE_DEBUG("btpan_close_handle : close handle %d", p->handle);
    p->handle = -1;
    p->local_role = -1;
    p->remote_role = -1;
    memset(&p->peer, 0, 6);
}
