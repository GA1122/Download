scoped_ptr<GDataEntryProtoVector> GDataDirectory::ToProtoVector() const {
  scoped_ptr<GDataEntryProtoVector> entries(new GDataEntryProtoVector);
  for (GDataFileCollection::const_iterator iter = child_files().begin();
       iter != child_files().end(); ++iter) {
    GDataEntryProto proto;
    iter->second->ToProto(&proto);
    entries->push_back(proto);
  }
  for (GDataDirectoryCollection::const_iterator iter =
           child_directories().begin();
       iter != child_directories().end(); ++iter) {
    GDataEntryProto proto;
    static_cast<const GDataEntry*>(iter->second)->ToProtoFull(&proto);
    entries->push_back(proto);
  }

  return entries.Pass();
}
