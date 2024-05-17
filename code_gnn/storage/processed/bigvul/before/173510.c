int omx_vdec::update_resolution(int width, int height, int stride, int scan_lines)
{
 int format_changed = 0;
 if ((height != (int)drv_ctx.video_resolution.frame_height) ||
 (width != (int)drv_ctx.video_resolution.frame_width)) {
        DEBUG_PRINT_HIGH("NOTE_CIF: W/H %d (%d), %d (%d)",
                width, drv_ctx.video_resolution.frame_width,
                height,drv_ctx.video_resolution.frame_height);
        format_changed = 1;
 }
    drv_ctx.video_resolution.frame_height = height;
    drv_ctx.video_resolution.frame_width = width;
    drv_ctx.video_resolution.scan_lines = scan_lines;
    drv_ctx.video_resolution.stride = stride;
 if(!is_down_scalar_enabled) {
    rectangle.nLeft = 0;
    rectangle.nTop = 0;
    rectangle.nWidth = drv_ctx.video_resolution.frame_width;
    rectangle.nHeight = drv_ctx.video_resolution.frame_height;
 }
 return format_changed;
}
