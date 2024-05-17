bool omx_venc::dev_get_peak_bitrate(OMX_U32 *peakbitrate)
{
#ifdef _MSM8974_
 return handle->venc_get_peak_bitrate(peakbitrate);
#else
    DEBUG_PRINT_ERROR("Get peak bitrate is not supported");
 return false;
#endif
}
