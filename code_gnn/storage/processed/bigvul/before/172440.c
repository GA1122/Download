bool venc_dev::venc_get_vui_timing_info(OMX_U32 *enabled)
{
 if (!enabled) {
        DEBUG_PRINT_ERROR("Null pointer error");
 return false;
 } else {
 *enabled = vui_timing_info.enabled;
 return true;
 }
}
