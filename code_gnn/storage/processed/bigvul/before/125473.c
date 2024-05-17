void GDataFileSystem::TransferFileFromLocalToRemoteAfterGetEntryInfo(
    const FilePath& local_src_file_path,
    const FilePath& remote_dest_file_path,
    const FileOperationCallback& callback,
    GDataFileError error,
    scoped_ptr<GDataEntryProto> entry_proto) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(!callback.is_null());

  if (error != GDATA_FILE_OK) {
    callback.Run(error);
    return;
  }

  DCHECK(entry_proto.get());
  if (!entry_proto->file_info().is_directory()) {
    callback.Run(GDATA_FILE_ERROR_NOT_A_DIRECTORY);
    return;
  }

  std::string* resource_id = new std::string;
  util::PostBlockingPoolSequencedTaskAndReply(
      FROM_HERE,
      blocking_task_runner_,
      base::Bind(&GetDocumentResourceIdOnBlockingPool,
                 local_src_file_path,
                 resource_id),
      base::Bind(&GDataFileSystem::TransferFileForResourceId,
                 ui_weak_ptr_,
                 local_src_file_path,
                 remote_dest_file_path,
                 callback,
                 base::Owned(resource_id)));
}
