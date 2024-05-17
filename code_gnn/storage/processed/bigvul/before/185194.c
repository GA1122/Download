 GDataEntry::GDataEntry(GDataDirectory* parent,
                       GDataDirectoryService* directory_service)
    : directory_service_(directory_service),
// GDataEntry::GDataEntry(GDataDirectoryService* directory_service)
//     : parent_(NULL),
//       directory_service_(directory_service),
        deleted_(false) {
  SetParent(parent);
  }