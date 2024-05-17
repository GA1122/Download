bool omx_venc::dev_is_video_session_supported(OMX_U32 width, OMX_U32 height)
{
#ifdef _MSM8974_
 return handle->venc_is_video_session_supported(width,height);
#else
    DEBUG_PRINT_LOW("Check against video capability not supported");
 return true;
#endif
}
