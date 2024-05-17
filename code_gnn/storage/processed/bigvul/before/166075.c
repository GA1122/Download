void FileSystemManagerImpl::GetPlatformPath(const GURL& path,
                                            GetPlatformPathCallback callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  base::FilePath platform_path;
  context_->default_file_task_runner()->PostTask(
      FROM_HERE,
      base::BindOnce(&FileSystemManagerImpl::GetPlatformPathOnFileThread, path,
                     process_id_, base::Unretained(context_), GetWeakPtr(),
                     std::move(callback)));
}
