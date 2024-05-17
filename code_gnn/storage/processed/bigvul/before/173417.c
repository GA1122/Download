void omx_vdec::buf_ref_add(OMX_U32 fd, OMX_U32 offset)
{
 unsigned long i = 0;
 bool buf_present = false;

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
               out_dynamic_list[i].ref_count++;
               DEBUG_PRINT_LOW("buf_ref_add: [ALREADY PRESENT] fd = %u ref_count = %u",
 (unsigned int)out_dynamic_list[i].fd, (unsigned int)out_dynamic_list[i].ref_count);
               buf_present = true;
 break;
 }
 }
 if (!buf_present) {
 for (i = 0; i < drv_ctx.op_buf.actualcount; i++) {
 if (out_dynamic_list[i].dup_fd == 0) {
                out_dynamic_list[i].fd = fd;
                out_dynamic_list[i].offset = offset;
                out_dynamic_list[i].dup_fd = dup(fd);
                out_dynamic_list[i].ref_count++;
                DEBUG_PRINT_LOW("buf_ref_add: [ADDED] fd = %u ref_count = %u",
 (unsigned int)out_dynamic_list[i].fd, (unsigned int)out_dynamic_list[i].ref_count);
 break;
 }
 }
 }
   pthread_mutex_unlock(&m_lock);
}
