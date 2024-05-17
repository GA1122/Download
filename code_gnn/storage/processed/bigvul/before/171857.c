static BOOLEAN  btif_hl_find_data_type_idx(UINT16 data_type, UINT8 *p_idx){
    BOOLEAN found = FALSE;
    UINT8 i;

 for (i=0; i< BTIF_HL_DATA_TABLE_SIZE; i++ )
 {
 if (data_type_table[i].data_type == data_type)
 {
            found = TRUE;
 *p_idx= i;
 break;
 }
 }

    BTIF_TRACE_DEBUG("%s found=%d, data_type=0x%x idx=%d", __FUNCTION__, found, data_type, i);
 return found;
}
