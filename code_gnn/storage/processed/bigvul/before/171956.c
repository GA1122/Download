btpan_conn_t* btpan_find_conn_addr(const BD_ADDR addr)
{
 for (int i = 0; i < MAX_PAN_CONNS; i++)
 {
 if (memcmp(btpan_cb.conns[i].peer, addr, sizeof(BD_ADDR)) == 0)
 return &btpan_cb.conns[i];
 }
 return NULL;
}
