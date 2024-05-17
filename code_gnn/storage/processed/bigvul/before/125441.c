void GDataFileSystem::RemoveOnUIThreadAfterGetEntryInfo(
    const FilePath& file_path,
    bool  ,
    const FileOperationCallback& callback,
    GDataFileError error,
    scoped_ptr<GDataEntryProto> entry_proto) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  if (error != GDATA_FILE_OK) {
    if (!callback.is_null()) {
      base::MessageLoopProxy::current()->PostTask(
          FROM_HERE, base::Bind(callback, error));
    }
    return;
  }

  DCHECK(entry_proto.get());
  documents_service_->DeleteDocument(
      GURL(entry_proto->edit_url()),
      base::Bind(&GDataFileSystem::OnRemovedDocument,
                 ui_weak_ptr_,
                 callback,
                 file_path));
}
