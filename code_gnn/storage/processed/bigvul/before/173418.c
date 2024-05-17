void omx_vdec::buf_ref_remove(OMX_U32 fd, OMX_U32 offset)
{
 unsigned long i = 0;

 if (!dynamic_buf_mode) {
 return;
 }

 if (!out_dynamic_list) {
        DEBUG_PRINT_ERROR("buf_ref_add: out_dynamic_list is NULL");
 return;
 }

    pthread_mutex_lock(&m_lock);
 for (i = 0; i < drv_ctx.op_buf.actualcount; i++) {
 if ((out_dynamic_list[i].fd == fd) &&
 (out_dynamic_list[i].offset == offset)) {
            out_dynamic_list[i].ref_count--;
 if (out_dynamic_list[i].ref_count == 0) {
                close(out_dynamic_list[i].dup_fd);
                DEBUG_PRINT_LOW("buf_ref_remove: [REMOVED] fd = %u ref_count = %u",
 (unsigned int)out_dynamic_list[i].fd, (unsigned int)out_dynamic_list[i].ref_count);
                out_dynamic_list[i].dup_fd = 0;
                out_dynamic_list[i].fd = 0;
                out_dynamic_list[i].offset = 0;
 }
 break;
 }
 }
 if (i  >= drv_ctx.op_buf.actualcount) {
        DEBUG_PRINT_ERROR("Error - could not remove ref, no match with any entry in list");
 }
    pthread_mutex_unlock(&m_lock);
}
