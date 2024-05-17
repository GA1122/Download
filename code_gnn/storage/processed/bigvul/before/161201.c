storage::FileSystemURL CreateNativeLocalFileSystemURL(
    storage::FileSystemContext* context,
    const base::FilePath local_path) {
  return context->CreateCrackedFileSystemURL(
      GURL(kOrigin), storage::kFileSystemTypeNativeLocal, local_path);
}
