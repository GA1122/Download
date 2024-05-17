void ChromeContentUtilityClient::OnRobustJPEGDecodeImage(
    const std::vector<unsigned char>& encoded_data,
    int request_id) {
  if (!encoded_data.empty()) {
    scoped_ptr<SkBitmap> decoded_image(gfx::JPEGCodecRobustSlow::Decode(
        &encoded_data[0], encoded_data.size()));
    if (!decoded_image.get() || decoded_image->empty()) {
      Send(new ChromeUtilityHostMsg_DecodeImage_Failed(request_id));
    } else {
      Send(new ChromeUtilityHostMsg_DecodeImage_Succeeded(*decoded_image,
                                                          request_id));
    }
  } else {
    Send(new ChromeUtilityHostMsg_DecodeImage_Failed(request_id));
  }
  ReleaseProcessIfNeeded();
}
