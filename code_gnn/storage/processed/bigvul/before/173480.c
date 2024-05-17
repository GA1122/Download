void post_message(omx_vdec *omx, unsigned char id)
{
 int ret_value;
    DEBUG_PRINT_LOW("omx_vdec: post_message %d pipe out%d", id,omx->m_pipe_out);
    ret_value = write(omx->m_pipe_out, &id, 1);
    DEBUG_PRINT_LOW("post_message to pipe done %d",ret_value);
}
