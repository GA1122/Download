void FileSystemManagerImpl::DidWriteSync(WriteSyncCallbackEntry* entry,
                                         base::File::Error result,
                                         int64_t bytes,
                                         bool complete) {
  entry->bytes += bytes;
  if (complete || result != base::File::FILE_OK)
    std::move(entry->callback).Run(entry->bytes, result);
}
