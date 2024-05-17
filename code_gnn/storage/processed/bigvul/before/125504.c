void GDataDirectoryService::GetEntryInfoByPath(
    const FilePath& path,
    const GetEntryInfoCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(!callback.is_null());

  scoped_ptr<GDataEntryProto> entry_proto;
  GDataFileError error = GDATA_FILE_ERROR_FAILED;

  GDataEntry* entry = FindEntryByPathSync(path);
  if (entry) {
    entry_proto.reset(new GDataEntryProto);
    entry->ToProtoFull(entry_proto.get());
    error = GDATA_FILE_OK;
  } else {
    error = GDATA_FILE_ERROR_NOT_FOUND;
  }

  base::MessageLoopProxy::current()->PostTask(
      FROM_HERE,
      base::Bind(callback, error, base::Passed(&entry_proto)));
}
