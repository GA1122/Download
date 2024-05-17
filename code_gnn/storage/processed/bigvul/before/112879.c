void GDataCache::GetFileOnUIThread(const std::string& resource_id,
                                   const std::string& md5,
                                   const GetFileFromCacheCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  base::PlatformFileError* error =
      new base::PlatformFileError(base::PLATFORM_FILE_OK);
  FilePath* cache_file_path = new FilePath;
  pool_->GetSequencedTaskRunner(sequence_token_)->PostTaskAndReply(
      FROM_HERE,
      base::Bind(&GDataCache::GetFile,
                 base::Unretained(this),
                 resource_id,
                 md5,
                 error,
                 cache_file_path),
      base::Bind(&RunGetFileFromCacheCallback,
                 callback,
                 base::Owned(error),
                 resource_id,
                 md5,
                 base::Owned(cache_file_path)));
}
