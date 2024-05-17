void GDataRootDirectory::FromProto(const GDataRootDirectoryProto& proto) {
  root_ = this;
  GDataDirectory::FromProto(proto.gdata_directory());
  largest_changestamp_ = proto.largest_changestamp();
}
