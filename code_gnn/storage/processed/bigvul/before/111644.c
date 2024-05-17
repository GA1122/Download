GDataDirectory::GDataDirectory(GDataDirectory* parent, GDataRootDirectory* root)
    : GDataEntry(parent, root), origin_(UNINITIALIZED) {
  file_info_.is_directory = true;
}
