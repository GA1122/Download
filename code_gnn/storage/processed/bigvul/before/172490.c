unsigned venc_dev::venc_stop_done(void)
{
 struct venc_msg venc_msg;
    free_extradata();
    venc_msg.msgcode=VEN_MSG_STOP;
    venc_msg.statuscode=VEN_S_SUCCESS;
    venc_handle->async_message_process(venc_handle,&venc_msg);
 return 0;
}
