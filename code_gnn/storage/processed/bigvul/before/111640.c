void GDataFile::FromProto(const GDataFileProto& proto) {
  DCHECK(!proto.gdata_entry().file_info().is_directory());
  GDataEntry::FromProto(proto.gdata_entry());
  kind_ = DocumentEntry::EntryKind(proto.kind());
  thumbnail_url_ = GURL(proto.thumbnail_url());
  alternate_url_ = GURL(proto.alternate_url());
  content_mime_type_ = proto.content_mime_type();
  etag_ = proto.etag();
  id_ = proto.id();
  file_md5_ = proto.file_md5();
  document_extension_ = proto.document_extension();
  is_hosted_document_ = proto.is_hosted_document();
}
