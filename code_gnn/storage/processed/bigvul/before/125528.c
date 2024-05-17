void GDataEntry::SerializeToString(std::string* serialized_proto) const {
  const GDataFile* file = AsGDataFileConst();
  const GDataDirectory* dir = AsGDataDirectoryConst();

  if (file) {
    GDataEntryProto entry_proto;
    file->ToProto(&entry_proto);
    const bool ok = entry_proto.SerializeToString(serialized_proto);
    DCHECK(ok);
  } else if (dir) {
    GDataDirectoryProto dir_proto;
    dir->ToProto(&dir_proto);
    const bool ok = dir_proto.SerializeToString(serialized_proto);
    DCHECK(ok);
  }
}
