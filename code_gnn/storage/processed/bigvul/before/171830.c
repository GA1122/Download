static void update_keyboard_lockstates(btif_hh_device_t *p_dev)
{
    UINT8 len = 2;  
    BD_ADDR* bda;
    BT_HDR* p_buf;
    UINT8 data[] = {0x01,  
                    btif_hh_keylockstates};  

  
    BTIF_TRACE_EVENT("%s: setting report on dev_handle %d to 0x%x",
         __FUNCTION__, p_dev->dev_handle, btif_hh_keylockstates);

  
    p_buf = create_pbuf(len, data);
 if (p_buf != NULL) {
        p_buf->layer_specific = BTA_HH_RPTT_OUTPUT;
        bda = (BD_ADDR*) (&p_dev->bd_addr);
        BTA_HhSendData(p_dev->dev_handle, *bda, p_buf);
 }
}
