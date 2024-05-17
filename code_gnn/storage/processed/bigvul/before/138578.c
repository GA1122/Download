void WriteFromUrlOperation::GetDownloadTarget(
    const base::Closure& continuation) {
  DCHECK_CURRENTLY_ON(BrowserThread::FILE);
  if (IsCancelled()) {
    return;
  }

  if (url_.ExtractFileName().empty()) {
    if (!base::CreateTemporaryFileInDir(temp_dir_.GetPath(), &image_path_)) {
      Error(error::kTempFileError);
      return;
    }
  } else {
    base::FilePath file_name =
        base::FilePath::FromUTF8Unsafe(url_.ExtractFileName());
    image_path_ = temp_dir_.GetPath().Append(file_name);
  }

  BrowserThread::PostTask(BrowserThread::FILE, FROM_HERE, continuation);
}
