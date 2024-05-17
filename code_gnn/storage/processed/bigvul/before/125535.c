void GDataEntry::ToProtoFull(GDataEntryProto* proto) const {
  if (AsGDataFileConst()) {
    AsGDataFileConst()->ToProto(proto);
  } else if (AsGDataDirectoryConst()) {
    ToProto(proto);
  } else {
    NOTREACHED();
  }
}
