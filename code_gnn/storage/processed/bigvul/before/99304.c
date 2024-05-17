void ResourceMessageFilter::OnGetFileInfoOnFileThread(
    const FilePath& path,
    IPC::Message* reply_msg,
    FileInfoWriteFunc write_func) {
  DCHECK(ChromeThread::CurrentlyOn(ChromeThread::FILE));

  file_util::FileInfo file_info;
  file_info.size = 0;
  file_util::GetFileInfo(path, &file_info);

  (*write_func)(reply_msg, file_info);

  ChromeThread::PostTask(
      ChromeThread::IO, FROM_HERE,
      NewRunnableMethod(this, &ResourceMessageFilter::Send, reply_msg));
}
