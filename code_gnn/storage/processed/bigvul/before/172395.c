bool omx_venc::dev_get_vui_timing_info(OMX_U32 *enabled)
{
#ifdef _MSM8974_
 return handle->venc_get_vui_timing_info(enabled);
#else
    DEBUG_PRINT_ERROR("Get vui timing information is not supported");
 return false;
#endif
}
