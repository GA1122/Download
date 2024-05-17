bool omx_vdec::ts_arr_list::reset_ts_list()
{
 bool ret = true;
 int idx = 0;

    DEBUG_PRINT_LOW("reset_ts_list(): Resetting timestamp array list");
 for ( ; idx < MAX_NUM_INPUT_OUTPUT_BUFFERS; idx++) {
        m_ts_arr_list[idx].valid = false;
 }
 return ret;
}
