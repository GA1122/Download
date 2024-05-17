bool GDataFile::FromProto(const GDataEntryProto& proto) {
  DCHECK(!proto.file_info().is_directory());

  if (!GDataEntry::FromProto(proto))
    return false;

  thumbnail_url_ = GURL(proto.file_specific_info().thumbnail_url());
  alternate_url_ = GURL(proto.file_specific_info().alternate_url());
  content_mime_type_ = proto.file_specific_info().content_mime_type();
  file_md5_ = proto.file_specific_info().file_md5();
  document_extension_ = proto.file_specific_info().document_extension();
  is_hosted_document_ = proto.file_specific_info().is_hosted_document();

  return true;
}
