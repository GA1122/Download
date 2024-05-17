scoped_ptr<GDataEntry> GDataEntry::FromProtoString(
    const std::string& serialized_proto) {
  scoped_ptr<GDataDirectoryProto> dir_proto(new GDataDirectoryProto());
  bool ok = dir_proto->ParseFromString(serialized_proto);
  if (ok && dir_proto->gdata_entry().file_info().is_directory()) {
    GDataDirectory* dir = new GDataDirectory(NULL, NULL);
    dir->FromProto(*dir_proto);
    return scoped_ptr<GDataEntry>(dir);
  }

  scoped_ptr<GDataFileProto> file_proto(new GDataFileProto());
  ok = file_proto->ParseFromString(serialized_proto);
  if (ok) {
    DCHECK(!file_proto->gdata_entry().file_info().is_directory());
    GDataFile* file = new GDataFile(NULL, NULL);
    file->FromProto(*file_proto);
    return scoped_ptr<GDataEntry>(file);
  }
  return scoped_ptr<GDataEntry>(NULL);
}
