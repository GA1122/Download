bool DownloadItemImpl::NeedsRename() const {
  DCHECK(target_path_.DirName() == current_path_.DirName());
  return target_path_ != current_path_;
}
