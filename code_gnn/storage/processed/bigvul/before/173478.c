bool omx_vdec::ts_arr_list::pop_min_ts(OMX_TICKS &ts)
{
 bool ret = true;
 int min_idx = -1;
    OMX_TICKS min_ts = 0;
 int idx = 0;

 for ( ; idx < MAX_NUM_INPUT_OUTPUT_BUFFERS; idx++) {

 if (m_ts_arr_list[idx].valid) {
 if (min_idx < 0) {
                min_ts = m_ts_arr_list[idx].timestamp;
                min_idx = idx;
 } else if (m_ts_arr_list[idx].timestamp < min_ts) {
                min_ts = m_ts_arr_list[idx].timestamp;
                min_idx = idx;
 }
 }

 }

 if (min_idx < 0) {
        DEBUG_PRINT_LOW("Timestamp array list is empty. Unsuccessful pop");
        ts = 0;
        ret = false;
 } else {
        ts = m_ts_arr_list[min_idx].timestamp;
        m_ts_arr_list[min_idx].valid = false;
        DEBUG_PRINT_LOW("Pop_min_ts:Timestamp (%lld), index(%d)",
                ts, min_idx);
 }

 return ret;

}
