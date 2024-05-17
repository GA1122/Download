void FileSystemManagerImpl::DidGetMetadataForStreaming(
    CreateSnapshotFileCallback callback,
    base::File::Error result,
    const base::File::Info& info) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  std::move(callback).Run(info, base::FilePath(), result, nullptr);
}
