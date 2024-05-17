bool venc_dev::venc_set_buf_req(OMX_U32 *min_buff_count,
        OMX_U32 *actual_buff_count,
        OMX_U32 *buff_size,
        OMX_U32 port)
{
 (void)min_buff_count, (void)buff_size;
 unsigned long temp_count = 0;

 if (port == 0) {
 if (*actual_buff_count > m_sInput_buff_property.mincount) {
            temp_count = m_sInput_buff_property.actualcount;
            m_sInput_buff_property.actualcount = *actual_buff_count;
            DEBUG_PRINT_LOW("I/P Count set to %u", (unsigned int)*actual_buff_count);
 }
 } else {
 if (*actual_buff_count > m_sOutput_buff_property.mincount) {
            temp_count = m_sOutput_buff_property.actualcount;
            m_sOutput_buff_property.actualcount = *actual_buff_count;
            DEBUG_PRINT_LOW("O/P Count set to %u", (unsigned int)*actual_buff_count);
 }
 }

 return true;

}
