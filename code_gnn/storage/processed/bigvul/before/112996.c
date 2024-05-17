MockDownloadFile* MockDownloadFileFactory::GetExistingFile(
    const DownloadId& id) {
  DCHECK(files_.find(id) != files_.end());
  return files_[id];
}
