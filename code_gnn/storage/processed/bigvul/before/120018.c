void FillDriveEntryPropertiesValue(const drive::ResourceEntry& entry_proto,
                                   bool shared_with_me,
                                   DriveEntryProperties* properties) {
  properties->shared_with_me.reset(new bool(shared_with_me));
  properties->shared.reset(new bool(entry_proto.shared()));

  const drive::PlatformFileInfoProto& file_info = entry_proto.file_info();
  properties->file_size.reset(new double(file_info.size()));
  properties->last_modified_time.reset(new double(
      base::Time::FromInternalValue(file_info.last_modified()).ToJsTime()));

  if (!entry_proto.has_file_specific_info())
    return;

  const drive::FileSpecificInfo& file_specific_info =
      entry_proto.file_specific_info();

  if (!entry_proto.resource_id().empty()) {
    properties->thumbnail_url.reset(
        new std::string("https://www.googledrive.com/thumb/" +
                        entry_proto.resource_id() + "?width=500&height=500"));
  }
  if (file_specific_info.has_image_width()) {
    properties->image_width.reset(
        new int(file_specific_info.image_width()));
  }
  if (file_specific_info.has_image_height()) {
    properties->image_height.reset(
        new int(file_specific_info.image_height()));
  }
  if (file_specific_info.has_image_rotation()) {
    properties->image_rotation.reset(
        new int(file_specific_info.image_rotation()));
  }
  properties->is_hosted.reset(
      new bool(file_specific_info.is_hosted_document()));
  properties->content_mime_type.reset(
      new std::string(file_specific_info.content_mime_type()));

  properties->is_pinned.reset(
      new bool(file_specific_info.cache_state().is_pinned()));
  properties->is_present.reset(
      new bool(file_specific_info.cache_state().is_present()));
}
