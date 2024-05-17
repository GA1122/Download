void GDataDirectory::FromProto(const GDataDirectoryProto& proto) {
  DCHECK(proto.gdata_entry().file_info().is_directory());
  GDataEntry::FromProto(proto.gdata_entry());
  refresh_time_ = base::Time::FromInternalValue(proto.refresh_time());
  start_feed_url_ = GURL(proto.start_feed_url());
  next_feed_url_ = GURL(proto.next_feed_url());
  upload_url_ = GURL(proto.upload_url());
  origin_ = ContentOrigin(proto.origin());
  for (int i = 0; i < proto.child_files_size(); ++i) {
    scoped_ptr<GDataFile> file(new GDataFile(this, root_));
    file->FromProto(proto.child_files(i));
    AddEntry(file.release());
  }
  for (int i = 0; i < proto.child_directories_size(); ++i) {
    scoped_ptr<GDataDirectory> dir(new GDataDirectory(this, root_));
    dir->FromProto(proto.child_directories(i));
    AddEntry(dir.release());
  }
}
