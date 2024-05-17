void ResourceMessageFilter::OnGetFileSize(const FilePath& path,
                                          IPC::Message* reply_msg) {
  if (!ChildProcessSecurityPolicy::GetInstance()->CanUploadFile(id(), path)) {
    ViewHostMsg_GetFileSize::WriteReplyParams(
        reply_msg, static_cast<int64>(-1));
    Send(reply_msg);
    return;
  }

  ChromeThread::PostTask(
      ChromeThread::FILE, FROM_HERE,
      NewRunnableMethod(
          this, &ResourceMessageFilter::OnGetFileInfoOnFileThread, path,
          reply_msg, &WriteFileSize));
}
