bool DocumentLoader::MaybeCreateArchive() {
  if (!IsArchiveMIMEType(response_.MimeType()))
    return false;

  DCHECK(GetResource());
  ArchiveResource* main_resource = fetcher_->CreateArchive(GetResource());
  if (!main_resource)
    return false;
  CommitNavigation(main_resource->MimeType(), main_resource->Url());
  if (!frame_)
    return false;

  scoped_refptr<SharedBuffer> data(main_resource->Data());
  data->ForEachSegment(
      [this](const char* segment, size_t segment_size, size_t segment_offset) {
        CommitData(segment, segment_size);
        return true;
      });
  return true;
}
