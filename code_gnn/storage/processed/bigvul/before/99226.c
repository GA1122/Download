void ResourceMessageFilter::OnAllocateTempFileForPrinting(
    IPC::Message* reply_msg) {
   ChromeThread::PostTask(
      ChromeThread::FILE, FROM_HERE,
      NewRunnableMethod(
          this, &ResourceMessageFilter::DoOnAllocateTempFileForPrinting,
          reply_msg));
}
