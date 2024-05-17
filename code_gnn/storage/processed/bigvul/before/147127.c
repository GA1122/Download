bool DocumentLoader::MaybeCreateArchive() {
  if (!IsArchiveMIMEType(response_.MimeType()))
    return false;

  DCHECK(main_resource_);
  ArchiveResource* main_resource =
      fetcher_->CreateArchive(main_resource_.Get());
  if (!main_resource)
    return false;
  EnsureWriter(main_resource->MimeType(), main_resource->Url());
  if (!frame_)
    return false;

  frame_->GetDocument()->EnforceSandboxFlags(
      kSandboxAll &
      ~(kSandboxPopups | kSandboxPropagatesToAuxiliaryBrowsingContexts));

  RefPtr<SharedBuffer> data(main_resource->Data());
  data->ForEachSegment(
      [this](const char* segment, size_t segment_size, size_t segment_offset) {
        CommitData(segment, segment_size);
        return true;
      });
  return true;
}
