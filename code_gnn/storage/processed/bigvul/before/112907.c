void GDataCache::StoreOnUIThread(const std::string& resource_id,
                                 const std::string& md5,
                                 const FilePath& source_path,
                                 FileOperationType file_operation_type,
                                 const CacheOperationCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  base::PlatformFileError* error =
      new base::PlatformFileError(base::PLATFORM_FILE_OK);
  pool_->GetSequencedTaskRunner(sequence_token_)->PostTaskAndReply(
      FROM_HERE,
      base::Bind(&GDataCache::Store,
                 base::Unretained(this),
                 resource_id,
                 md5,
                 source_path,
                 file_operation_type,
                 error),
      base::Bind(&RunCacheOperationCallback,
                 callback,
                 base::Owned(error),
                 resource_id,
                 md5));
}
