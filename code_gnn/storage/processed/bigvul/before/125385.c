void GDataFileSystem::MoveOnUIThreadAfterGetEntryInfoPair(
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

  scoped_ptr<GDataEntryProto> dest_parent_proto = result->second.proto.Pass();
  if (!dest_parent_proto->file_info().is_directory()) {
    callback.Run(GDATA_FILE_ERROR_NOT_A_DIRECTORY);
    return;
  }

  const FilePath& src_file_path = result->first.path;
  const FilePath& dest_parent_path = result->second.path;
  if (src_file_path.DirName() == dest_parent_path) {
    FileMoveCallback final_file_path_update_callback =
        base::Bind(&GDataFileSystem::OnFilePathUpdated,
                   ui_weak_ptr_,
                   callback);

    Rename(src_file_path, dest_file_path.BaseName().value(),
           final_file_path_update_callback);
    return;
  }

  FileMoveCallback add_file_to_directory_callback =
      base::Bind(&GDataFileSystem::MoveEntryFromRootDirectory,
                 ui_weak_ptr_,
                 dest_file_path.DirName(),
                 callback);

  FileMoveCallback remove_file_from_directory_callback =
      base::Bind(&GDataFileSystem::RemoveEntryFromDirectory,
                 ui_weak_ptr_,
                 src_file_path.DirName(),
                 add_file_to_directory_callback);

  Rename(src_file_path, dest_file_path.BaseName().value(),
         remove_file_from_directory_callback);
}
