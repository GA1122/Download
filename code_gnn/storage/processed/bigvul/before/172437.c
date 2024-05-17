bool venc_dev::venc_get_performance_level(OMX_U32 *perflevel)
{
 if (!perflevel) {
        DEBUG_PRINT_ERROR("Null pointer error");
 return false;
 } else {
 *perflevel = performance_level.perflevel;
 return true;
 }
}
