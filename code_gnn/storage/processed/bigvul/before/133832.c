void FinishParseMediaMetadata(
    metadata::MediaMetadataParser*  ,
    const extensions::api::media_galleries::MediaMetadata& metadata,
    const std::vector<metadata::AttachedImage>& attached_images) {
  Send(new ChromeUtilityHostMsg_ParseMediaMetadata_Finished(
      true, *metadata.ToValue(), attached_images));
  ReleaseProcessIfNeeded();
}
