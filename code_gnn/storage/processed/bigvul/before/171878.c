static UINT8 btif_hl_get_next_app_id(){
    UINT8 next_app_id = btif_hl_cb.next_app_id;

    btif_hl_cb.next_app_id++;
 return next_app_id;
}
