btpan_conn_t* btpan_new_conn(int handle, const BD_ADDR addr, int local_role, int remote_role)
{
 for (int i = 0; i < MAX_PAN_CONNS; i++)
 {
        BTIF_TRACE_DEBUG("conns[%d]:%d", i, btpan_cb.conns[i].handle);
 if (btpan_cb.conns[i].handle == -1)
 {
            BTIF_TRACE_DEBUG("handle:%d, local_role:%d, remote_role:%d", handle, local_role, remote_role);

            btpan_cb.conns[i].handle = handle;
            bdcpy(btpan_cb.conns[i].peer, addr);
            btpan_cb.conns[i].local_role = local_role;
            btpan_cb.conns[i].remote_role = remote_role;
 return &btpan_cb.conns[i];
 }
 }
    BTIF_TRACE_DEBUG("MAX_PAN_CONNS:%d exceeded, return NULL as failed", MAX_PAN_CONNS);
 return NULL;
}
