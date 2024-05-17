void GDataFile::ToProto(GDataEntryProto* proto) const {
  GDataEntry::ToProto(proto);
  DCHECK(!proto->file_info().is_directory());
  GDataFileSpecificInfo* file_specific_info =
      proto->mutable_file_specific_info();
  file_specific_info->set_thumbnail_url(thumbnail_url_.spec());
  file_specific_info->set_alternate_url(alternate_url_.spec());
  file_specific_info->set_content_mime_type(content_mime_type_);
  file_specific_info->set_file_md5(file_md5_);
  file_specific_info->set_document_extension(document_extension_);
  file_specific_info->set_is_hosted_document(is_hosted_document_);
}
