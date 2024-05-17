bool GDataDirectoryService::ParseFromString(
    const std::string& serialized_proto) {
  GDataRootDirectoryProto proto;
  if (!proto.ParseFromString(serialized_proto))
    return false;

  if (proto.version() != kProtoVersion) {
    LOG(ERROR) << "Incompatible proto detected (incompatible version): "
               << proto.version();
    return false;
  }

  if (!IsValidRootDirectoryProto(proto.gdata_directory()))
    return false;

  if (!root_->FromProto(proto.gdata_directory()))
    return false;

  origin_ = FROM_CACHE;
  largest_changestamp_ = proto.largest_changestamp();

  return true;
}
