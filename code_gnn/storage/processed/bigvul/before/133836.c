void ChromeContentUtilityClient::OnParseMediaMetadata(
    const std::string& mime_type, int64 total_size, bool get_attached_images) {
  metadata::IPCDataSource* source = new metadata::IPCDataSource(total_size);
  handlers_.push_back(source);

  metadata::MediaMetadataParser* parser = new metadata::MediaMetadataParser(
      source, mime_type, get_attached_images);
  parser->Start(base::Bind(&FinishParseMediaMetadata, base::Owned(parser)));
}
