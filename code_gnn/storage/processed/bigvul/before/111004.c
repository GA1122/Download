void FileSystemOperation::DidEnsureFileExistsNonExclusive(
    const StatusCallback& callback,
    base::PlatformFileError rv, bool  ) {
  DidFinishFileOperation(callback, rv);
}
