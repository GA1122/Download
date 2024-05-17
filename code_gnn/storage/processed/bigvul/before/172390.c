bool omx_venc::dev_get_capability_ltrcount(OMX_U32 *min, OMX_U32 *max, OMX_U32 *step_size)
{
#ifdef _MSM8974_
 (void) min;
 (void) max;
 (void) step_size;
    DEBUG_PRINT_ERROR("Get Capability LTR Count is not supported");
 return false;
#else
 return handle->venc_get_capability_ltrcount(min, max, step_size);
#endif
}
