void DownloadFileCancel(std::unique_ptr<DownloadFile> download_file) {
  DCHECK(GetDownloadTaskRunner()->RunsTasksInCurrentSequence());
  download_file->Cancel();
}
