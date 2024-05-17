 void CheckDecodedFrames() {
    libvpx_test::DxDataIterator dec_iter = decoder_->GetDxData();
 const vpx_image_t *img = NULL;

 while ((img = dec_iter.Next()) != NULL) {
      fb_list_.CheckXImageFrameBuffer(img);
 }
 }
