void GDataFileSystem::OnGetEntryInfoCompleteForGetFileByPath(
    const FilePath& file_path,
    const GetFileCallback& get_file_callback,
    const GetDownloadDataCallback& get_download_data_callback,
    GDataFileError error,
    scoped_ptr<GDataEntryProto> entry_proto) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  DCHECK(error != GDATA_FILE_OK ||
         (entry_proto.get() && !entry_proto->resource_id().empty()));
  GetResolvedFileByPath(file_path,
                        get_file_callback,
                        get_download_data_callback,
                        error,
                        entry_proto.get());
}
