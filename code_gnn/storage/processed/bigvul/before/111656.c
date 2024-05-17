void GDataEntry::SerializeToString(std::string* serialized_proto) const {
  const GDataFile* file = AsGDataFileConst();
  const GDataDirectory* dir = AsGDataDirectoryConst();

  if (file) {
    scoped_ptr<GDataFileProto> proto(new GDataFileProto());
    file->ToProto(proto.get());
    const bool ok = proto->SerializeToString(serialized_proto);
    DCHECK(ok);
  } else if (dir) {
    scoped_ptr<GDataDirectoryProto> proto(new GDataDirectoryProto());
    dir->ToProto(proto.get());
    const bool ok = proto->SerializeToString(serialized_proto);
    DCHECK(ok);
  }
}
