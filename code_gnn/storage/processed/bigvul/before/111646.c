GDataFile::GDataFile(GDataDirectory* parent, GDataRootDirectory* root)
    : GDataEntry(parent, root),
      kind_(DocumentEntry::UNKNOWN),
      is_hosted_document_(false) {
  file_info_.is_directory = false;
}
