void GetLegacyLocalStorageUsage(
    const base::FilePath& directory,
    scoped_refptr<base::SingleThreadTaskRunner> reply_task_runner,
    DOMStorageContext::GetLocalStorageUsageCallback callback) {
  std::vector<StorageUsageInfo> infos;
  base::FileEnumerator enumerator(directory, false,
                                  base::FileEnumerator::FILES);
  for (base::FilePath path = enumerator.Next(); !path.empty();
       path = enumerator.Next()) {
    if (path.MatchesExtension(DOMStorageArea::kDatabaseFileExtension)) {
      base::FileEnumerator::FileInfo find_info = enumerator.GetInfo();
      infos.emplace_back(DOMStorageArea::OriginFromDatabaseFileName(path),
                         find_info.GetSize(), find_info.GetLastModifiedTime());
    }
  }
  reply_task_runner->PostTask(
      FROM_HERE, base::BindOnce(std::move(callback), std::move(infos)));
}
