void RunGetEntryInfoWithFilePathCallback(
    const GetEntryInfoWithFilePathCallback& callback,
    const FilePath& path,
    GDataFileError error,
    scoped_ptr<GDataEntryProto> entry_proto) {
  if (!callback.is_null())
    callback.Run(error, path, entry_proto.Pass());
}
