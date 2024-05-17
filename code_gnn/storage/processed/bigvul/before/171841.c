void btif_hl_copy_bda(bt_bdaddr_t *bd_addr, BD_ADDR  bda){
    UINT8 i;
 for (i=0; i<6; i++)
 {
        bd_addr->address[i] = bda[i] ;
 }
}
