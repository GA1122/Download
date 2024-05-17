void GDataFile::ToProto(GDataFileProto* proto) const {
  GDataEntry::ToProto(proto->mutable_gdata_entry());
  DCHECK(!proto->gdata_entry().file_info().is_directory());
  proto->set_kind(kind_);
  proto->set_thumbnail_url(thumbnail_url_.spec());
  proto->set_alternate_url(alternate_url_.spec());
  proto->set_content_mime_type(content_mime_type_);
  proto->set_etag(etag_);
  proto->set_id(id_);
  proto->set_file_md5(file_md5_);
  proto->set_document_extension(document_extension_);
  proto->set_is_hosted_document(is_hosted_document_);
}
