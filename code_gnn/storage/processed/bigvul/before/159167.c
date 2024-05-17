base::FilePath MakeCopyOfDownloadFile(DownloadFile* download_file) {
  DCHECK(GetDownloadTaskRunner()->RunsTasksInCurrentSequence());
  base::FilePath temp_file_path;
  if (!base::CreateTemporaryFile(&temp_file_path))
    return base::FilePath();

  if (!base::CopyFile(download_file->FullPath(), temp_file_path)) {
    DeleteDownloadedFile(temp_file_path);
    return base::FilePath();
  }

  return temp_file_path;
}
