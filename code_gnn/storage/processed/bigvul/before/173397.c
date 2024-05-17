omx_vdec::allocate_color_convert_buf::allocate_color_convert_buf()
{
    enabled = false;
    omx = NULL;
    init_members();
 ColorFormat = OMX_COLOR_FormatMax;
    dest_format = YCbCr420P;
}
