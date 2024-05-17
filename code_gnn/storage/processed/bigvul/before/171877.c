UINT16  btif_hl_get_max_tx_apdu_size(tBTA_HL_MDEP_ROLE mdep_role,
                                     UINT16 data_type ){
    UINT8 idx;
    UINT16 max_tx_apdu_size =0;

 if (btif_hl_find_data_type_idx(data_type, &idx))
 {
 if (mdep_role == BTA_HL_MDEP_ROLE_SOURCE)
 {
            max_tx_apdu_size = data_type_table[idx].max_tx_apdu_size;
 }
 else
 {
            max_tx_apdu_size = data_type_table[idx].max_rx_apdu_size;
 }
 }
 else
 {
 if (mdep_role == BTA_HL_MDEP_ROLE_SOURCE)
 {
            max_tx_apdu_size = BTIF_HL_DEFAULT_SRC_TX_APDU_SIZE;
 }
 else
 {
            max_tx_apdu_size = BTIF_HL_DEFAULT_SRC_RX_APDU_SIZE;
 }


 }

    BTIF_TRACE_DEBUG("%s mdep_role=%d data_type=0x%4x size=%d",
                      __FUNCTION__, mdep_role, data_type, max_tx_apdu_size);
 return max_tx_apdu_size;
}
