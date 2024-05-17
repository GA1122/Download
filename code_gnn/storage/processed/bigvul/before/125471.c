void GDataFileSystem::TransferFileForResourceId(
    const FilePath& local_file_path,
    const FilePath& remote_dest_file_path,
    const FileOperationCallback& callback,
    std::string* resource_id) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(resource_id);
  DCHECK(!callback.is_null());

  if (resource_id->empty()) {
    TransferRegularFile(local_file_path, remote_dest_file_path, callback);
    return;
  }

  CopyDocumentToDirectory(
      remote_dest_file_path.DirName(),
      *resource_id,
      remote_dest_file_path.BaseName().RemoveExtension().value(),
      callback);
}
