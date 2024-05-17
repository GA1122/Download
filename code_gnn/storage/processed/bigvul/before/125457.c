void RunFileOperationCallbackHelper(
    const FileOperationCallback& callback,
    GDataFileError* error) {
  DCHECK(error);

  if (!callback.is_null())
    callback.Run(*error);
}
