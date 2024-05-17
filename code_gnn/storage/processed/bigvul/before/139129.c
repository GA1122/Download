IPC::PlatformFileForTransit CreateFileForProcess(base::FilePath file_path) {
  DCHECK_CURRENTLY_ON(BrowserThread::FILE);
  base::File dump_file(file_path,
                       base::File::FLAG_OPEN_ALWAYS | base::File::FLAG_APPEND);
  if (!dump_file.IsValid()) {
    VLOG(1) << "Could not open AEC dump file, error="
            << dump_file.error_details();
    return IPC::InvalidPlatformFileForTransit();
  }
  return IPC::TakePlatformFileForTransit(std::move(dump_file));
}
