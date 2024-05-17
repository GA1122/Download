int omx_video::omx_c2d_conv::get_src_format()
{
 int format = -1;
 if (src_format == NV12_128m) {
        format = HAL_PIXEL_FORMAT_NV12_ENCODEABLE;
 } else if (src_format == RGBA8888) {
        format = HAL_PIXEL_FORMAT_RGBA_8888;
 }
 return format;
}
