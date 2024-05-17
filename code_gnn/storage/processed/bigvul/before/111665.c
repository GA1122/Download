void GDataRootDirectory::ToProto(GDataRootDirectoryProto* proto) const {
  GDataDirectory::ToProto(proto->mutable_gdata_directory());
  proto->set_largest_changestamp(largest_changestamp_);
}
