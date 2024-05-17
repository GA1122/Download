void ResourceMessageFilter::OnOpenFile(const FilePath& path,
                                       int mode,
                                       IPC::Message* reply_msg) {
  if (!ChildProcessSecurityPolicy::GetInstance()->CanUploadFile(id(), path)) {
    ViewHostMsg_OpenFile::WriteReplyParams(
        reply_msg, base::kInvalidPlatformFileValue);
    Send(reply_msg);
    return;
  }

  ChromeThread::PostTask(
      ChromeThread::FILE, FROM_HERE,
      NewRunnableMethod(
          this, &ResourceMessageFilter::OnOpenFileOnFileThread,
          path, mode, reply_msg));
}
