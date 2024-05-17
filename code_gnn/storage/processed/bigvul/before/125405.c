void GDataFileSystem::OnGetEntryInfo(const GetEntryInfoCallback& callback,
                                    GDataFileError error,
                                    GDataEntry* entry) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  if (error != GDATA_FILE_OK) {
    if (!callback.is_null())
      callback.Run(error, scoped_ptr<GDataEntryProto>());
    return;
  }
  DCHECK(entry);

  scoped_ptr<GDataEntryProto> entry_proto(new GDataEntryProto);
  entry->ToProtoFull(entry_proto.get());

  CheckLocalModificationAndRun(entry_proto.Pass(), callback);
}
