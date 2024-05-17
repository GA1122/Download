void DeleteDownloadedFileOnUIThread(const base::FilePath& file_path) {
  if (!file_path.empty()) {
    download::GetDownloadTaskRunner()->PostTask(
        FROM_HERE,
        base::BindOnce(base::IgnoreResult(&download::DeleteDownloadedFile),
                       file_path));
  }
}
