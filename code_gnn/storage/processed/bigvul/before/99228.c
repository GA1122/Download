void ResourceMessageFilter::OnClipboardReadAsciiText(Clipboard::Buffer buffer,
                                                     IPC::Message* reply_msg) {
  ChromeThread::PostTask(
      ChromeThread::UI, FROM_HERE,
      NewRunnableMethod(
          this, &ResourceMessageFilter::DoOnClipboardReadAsciiText, buffer,
          reply_msg));
}
