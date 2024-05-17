void GDataFileSystem::GetEntryInfoByEntryOnUIThread(
    const GetEntryInfoWithFilePathCallback& callback,
    GDataEntry* entry) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  if (entry) {
    scoped_ptr<GDataEntryProto> entry_proto(new GDataEntryProto);
    entry->ToProtoFull(entry_proto.get());
    CheckLocalModificationAndRun(
        entry_proto.Pass(),
        base::Bind(&RunGetEntryInfoWithFilePathCallback,
                   callback, entry->GetFilePath()));
  } else {
    callback.Run(GDATA_FILE_ERROR_NOT_FOUND,
                 FilePath(),
                 scoped_ptr<GDataEntryProto>());
  }
}
