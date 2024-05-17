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
  for (const auto& span : *data)
    CommitData(span.data(), span.size());
  return true;
}
