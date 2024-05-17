SkBitmap ChromeContentUtilityClient::DecodeImage(
    const std::vector<unsigned char>& encoded_data, bool shrink_to_fit) {
  SkBitmap decoded_image;
  if (encoded_data.empty())
    return decoded_image;

  decoded_image = content::DecodeImage(&encoded_data[0],
                                       gfx::Size(),
                                       encoded_data.size());

  int64_t struct_size = sizeof(ChromeUtilityHostMsg_DecodeImage_Succeeded);
  int64_t image_size = decoded_image.computeSize64();
  int halves = 0;
  while (struct_size + (image_size >> 2*halves) > max_ipc_message_size_)
    halves++;
  if (halves) {
    if (shrink_to_fit) {
      decoded_image = skia::ImageOperations::Resize(
          decoded_image, skia::ImageOperations::RESIZE_LANCZOS3,
          decoded_image.width() >> halves, decoded_image.height() >> halves);
    } else {
      decoded_image.reset();
      LOG(ERROR) << "Decoded image too large for IPC message";
    }
  }

  return decoded_image;
}
