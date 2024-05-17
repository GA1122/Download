GDataEntry* GDataDirectory::FromDocumentEntry(GDataDirectory* parent,
                                              DocumentEntry* doc,
                                              GDataRootDirectory* root) {
  DCHECK(doc->is_folder());
  GDataDirectory* dir = new GDataDirectory(parent, root);
  dir->title_ = UTF16ToUTF8(doc->title());
  dir->SetFileNameFromTitle();
  dir->file_info_.last_modified = doc->updated_time();
  dir->file_info_.last_accessed = doc->updated_time();
  dir->file_info_.creation_time = doc->published_time();
  dir->start_feed_url_ = doc->content_url();
  dir->resource_id_ = doc->resource_id();
  dir->content_url_ = doc->content_url();
  dir->deleted_ = doc->deleted();

  const Link* edit_link = doc->GetLinkByType(Link::EDIT);
  DCHECK(edit_link) << "No edit link for dir " << dir->title_;
  if (edit_link)
    dir->edit_url_ = edit_link->href();

  const Link* parent_link = doc->GetLinkByType(Link::PARENT);
  if (parent_link)
    dir->parent_resource_id_ = ExtractResourceId(parent_link->href());

  const Link* upload_link = doc->GetLinkByType(Link::RESUMABLE_CREATE_MEDIA);
  if (upload_link)
    dir->upload_url_ = upload_link->href();

  return dir;
}
