base::FilePath DownloadFileDetach(std::unique_ptr<DownloadFile> download_file) {
  DCHECK(GetDownloadTaskRunner()->RunsTasksInCurrentSequence());
  base::FilePath full_path = download_file->FullPath();
  download_file->Detach();
  return full_path;
}
