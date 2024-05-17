 GDataDirectory::GDataDirectory(GDataDirectory* parent,
                               GDataDirectoryService* directory_service)
    : GDataEntry(parent, directory_service) {
// GDataDirectory::GDataDirectory(GDataDirectoryService* directory_service)
//     : GDataEntry(directory_service) {
    file_info_.is_directory = true;
  }