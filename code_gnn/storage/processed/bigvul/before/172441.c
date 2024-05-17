bool venc_dev::venc_is_video_session_supported(unsigned long width,
 unsigned long height)
{
 if ((width * height < capability.min_width *  capability.min_height) ||
 (width * height > capability.max_width *  capability.max_height)) {
        DEBUG_PRINT_ERROR(
 "Unsupported video resolution WxH = (%lu)x(%lu) supported range = min (%d)x(%d) - max (%d)x(%d)",
                width, height, capability.min_width, capability.min_height,
                capability.max_width, capability.max_height);
 return false;
 }

    DEBUG_PRINT_LOW("video session supported");
 return true;
}
