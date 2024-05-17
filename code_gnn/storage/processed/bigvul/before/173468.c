bool omx_vdec::ts_arr_list::insert_ts(OMX_TICKS ts)
{
 bool ret = true;
 bool duplicate_ts = false;
 int idx = 0;

 for ( ; idx < MAX_NUM_INPUT_OUTPUT_BUFFERS; idx++) {
 if (!m_ts_arr_list[idx].valid) {
            m_ts_arr_list[idx].valid = true;
            m_ts_arr_list[idx].timestamp = ts;
            DEBUG_PRINT_LOW("Insert_ts(): Inserting TIMESTAMP (%lld) at idx (%d)",
                    ts, idx);
 break;
 }
 }

 if (idx == MAX_NUM_INPUT_OUTPUT_BUFFERS) {
        DEBUG_PRINT_LOW("Timestamp array list is FULL. Unsuccessful insert");
        ret = false;
 }
 return ret;
}
