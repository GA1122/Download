void post_message(omx_video *omx, unsigned char id)
{
    DEBUG_PRINT_LOW("omx_venc: post_message %d", id);
    write(omx->m_pipe_out, &id, 1);
}
