bool DirectoryBackingStore::BeginLoad() {
  DCHECK(load_dbhandle_ == NULL);
  bool ret = OpenAndConfigureHandleHelper(&load_dbhandle_);
  if (ret)
    return true;
  using ::operator<<;   
  LOG(ERROR) << "Sync database " << backing_filepath_.value()
             << " corrupt. Deleting and recreating.";
  file_util::Delete(backing_filepath_, false);
  bool failed_again = !OpenAndConfigureHandleHelper(&load_dbhandle_);

  int bucket = failed_again ? 2 : 1;
#if defined(OS_WIN)
  UMA_HISTOGRAM_COUNTS_100("Sync.DirectoryOpenFailedWin", bucket);
#elif defined(OS_MACOSX)
  UMA_HISTOGRAM_COUNTS_100("Sync.DirectoryOpenFailedMac", bucket);
#else
  UMA_HISTOGRAM_COUNTS_100("Sync.DirectoryOpenFailedNotWinMac", bucket);

#if defined(OS_LINUX) && !defined(OS_CHROMEOS)
  UMA_HISTOGRAM_COUNTS_100("Sync.DirectoryOpenFailedLinux", bucket);
#elif defined(OS_CHROMEOS)
  UMA_HISTOGRAM_COUNTS_100("Sync.DirectoryOpenFailedCros", bucket);
#else
  UMA_HISTOGRAM_COUNTS_100("Sync.DirectoryOpenFailedOther", bucket);
#endif   
#endif   
  return !failed_again;
}
