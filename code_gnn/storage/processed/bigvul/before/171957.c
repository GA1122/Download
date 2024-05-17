btpan_conn_t * btpan_find_conn_handle(UINT16 handle)
{
 for (int i = 0; i < MAX_PAN_CONNS; i++)
 {
 if (btpan_cb.conns[i].handle == handle)
 return &btpan_cb.conns[i];
 }
 return NULL;
}
