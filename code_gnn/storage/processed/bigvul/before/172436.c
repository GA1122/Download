bool venc_dev::venc_get_peak_bitrate(OMX_U32 *peakbitrate)
{
 if (!peakbitrate) {
        DEBUG_PRINT_ERROR("Null pointer error");
 return false;
 } else {
 *peakbitrate = peak_bitrate.peakbitrate;
 return true;
 }
}
