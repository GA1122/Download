bool omx_venc::dev_get_performance_level(OMX_U32 *perflevel)
{
#ifdef _MSM8974_
 return handle->venc_get_performance_level(perflevel);
#else
    DEBUG_PRINT_ERROR("Get performance level is not supported");
 return false;
#endif
}
