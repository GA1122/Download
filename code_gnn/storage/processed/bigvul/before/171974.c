BOOLEAN btif_rc_get_connected_peer(BD_ADDR peer_addr)
{
 if (btif_rc_cb.rc_connected == TRUE) {
        bdcpy(peer_addr, btif_rc_cb.rc_addr);
 return TRUE;
 }
 return FALSE;
}
