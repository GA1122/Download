unsigned venc_dev::venc_start_done(void)
{
 struct venc_msg venc_msg;
    venc_msg.msgcode = VEN_MSG_START;
    venc_msg.statuscode = VEN_S_SUCCESS;
    venc_handle->async_message_process(venc_handle,&venc_msg);
 return 0;
}
