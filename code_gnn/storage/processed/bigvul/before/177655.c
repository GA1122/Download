 virtual void DecompressedFrameHook(const vpx_image_t &img,
 vpx_codec_pts_t pts) {
    frame_info_list_.push_back(FrameInfo(pts, img.d_w, img.d_h));
 }