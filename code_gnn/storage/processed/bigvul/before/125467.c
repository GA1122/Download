void GDataFileSystem::StartFileUploadOnUIThreadAfterGetEntryInfo(
    const StartFileUploadParams& params,
    int64 file_size,
    std::string content_type,
    GDataFileError error,
    scoped_ptr<GDataEntryProto> entry_proto) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  if (entry_proto.get() && !entry_proto->file_info().is_directory())
    error = GDATA_FILE_ERROR_NOT_A_DIRECTORY;

  if (error != GDATA_FILE_OK) {
    if (!params.callback.is_null())
      params.callback.Run(error);
    return;
  }
  DCHECK(entry_proto.get());

  scoped_ptr<UploadFileInfo> upload_file_info(new UploadFileInfo);
  upload_file_info->file_path = params.local_file_path;
  upload_file_info->file_size = file_size;
  upload_file_info->gdata_path = params.remote_file_path;
  upload_file_info->title = params.remote_file_path.BaseName().value();
  upload_file_info->content_length = file_size;
  upload_file_info->all_bytes_present = true;
  upload_file_info->content_type = content_type;
  upload_file_info->initial_upload_location = GURL(entry_proto->upload_url());

  upload_file_info->completion_callback =
      base::Bind(&GDataFileSystem::OnTransferCompleted,
                 ui_weak_ptr_,
                 params.callback);

  uploader_->UploadNewFile(upload_file_info.Pass());
}
