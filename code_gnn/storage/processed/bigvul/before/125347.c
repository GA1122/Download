void GDataFileSystem::CopyOnUIThreadAfterGetEntryInfoPair(
    const FilePath& dest_file_path,
    const FileOperationCallback& callback,
    scoped_ptr<EntryInfoPairResult> result) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(!callback.is_null());
  DCHECK(result.get());

  if (result->first.error != GDATA_FILE_OK) {
    callback.Run(result->first.error);
    return;
  } else if (result->second.error != GDATA_FILE_OK) {
    callback.Run(result->second.error);
    return;
  }

  scoped_ptr<GDataEntryProto> src_file_proto = result->first.proto.Pass();
  scoped_ptr<GDataEntryProto> dest_parent_proto = result->second.proto.Pass();

  if (!dest_parent_proto->file_info().is_directory()) {
    callback.Run(GDATA_FILE_ERROR_NOT_A_DIRECTORY);
    return;
  } else if (src_file_proto->file_info().is_directory()) {
    callback.Run(GDATA_FILE_ERROR_INVALID_OPERATION);
    return;
  }

  if (src_file_proto->file_specific_info().is_hosted_document()) {
    CopyDocumentToDirectory(dest_file_path.DirName(),
                            src_file_proto->resource_id(),
                            dest_file_path.BaseName().RemoveExtension().value(),
                            callback);
    return;
  }

  const FilePath& src_file_path = result->first.path;
  GetFileByPath(src_file_path,
                base::Bind(&GDataFileSystem::OnGetFileCompleteForCopy,
                           ui_weak_ptr_,
                           dest_file_path,
                           callback),
                GetDownloadDataCallback());
}
