void ChromeContentUtilityClient::DecodeImageAndSend(
    const std::vector<unsigned char>& encoded_data,
    bool shrink_to_fit,
    int request_id) {
  SkBitmap decoded_image = DecodeImage(encoded_data, shrink_to_fit);

  if (decoded_image.empty()) {
    Send(new ChromeUtilityHostMsg_DecodeImage_Failed(request_id));
  } else {
    Send(new ChromeUtilityHostMsg_DecodeImage_Succeeded(decoded_image,
                                                        request_id));
  }
  ReleaseProcessIfNeeded();
}
