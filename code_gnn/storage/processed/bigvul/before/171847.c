void btif_hl_display_bt_bda(bt_bdaddr_t *bd_addr){
    BTIF_TRACE_DEBUG("DB [%02x:%02x:%02x:%02x:%02x:%02x]",
                      bd_addr->address[0],   bd_addr->address[1], bd_addr->address[2],
                      bd_addr->address[3],  bd_addr->address[4],   bd_addr->address[5]);
}
