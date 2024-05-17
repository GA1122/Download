void GDataFile::SetBaseNameFromTitle() {
  if (is_hosted_document_) {
    base_name_ = EscapeUtf8FileName(title_ + document_extension_);
  } else {
    GDataEntry::SetBaseNameFromTitle();
   }
 }
