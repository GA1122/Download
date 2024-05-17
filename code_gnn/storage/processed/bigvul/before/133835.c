void ChromeContentUtilityClient::OnDecodeImage(
    const std::vector<unsigned char>& encoded_data,
    bool shrink_to_fit,
    int request_id) {
  content::UtilityThread::Get()->EnsureBlinkInitialized();
  DecodeImageAndSend(encoded_data, shrink_to_fit, request_id);
}
