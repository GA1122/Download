bool IsValidRootDirectoryProto(const GDataDirectoryProto& proto) {
  const GDataEntryProto& entry_proto = proto.gdata_entry();
  if (entry_proto.title() != "drive") {
    LOG(ERROR) << "Incompatible proto detected (bad title): "
               << entry_proto.title();
    return false;
  }
  if (entry_proto.resource_id() != kGDataRootDirectoryResourceId) {
    LOG(ERROR) << "Incompatible proto detected (bad resource ID): "
               << entry_proto.resource_id();
    return false;
  }

  return true;
}
