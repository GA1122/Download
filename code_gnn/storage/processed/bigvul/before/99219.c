void ResourceMessageFilter::DoOnClipboardReadAsciiText(
    Clipboard::Buffer buffer, IPC::Message* reply_msg) {
  std::string result;
  GetClipboard()->ReadAsciiText(buffer, &result);

  ViewHostMsg_ClipboardReadAsciiText::WriteReplyParams(reply_msg, result);

   ChromeThread::PostTask(
      ChromeThread::IO, FROM_HERE,
      NewRunnableMethod(
          this, &ResourceMessageFilter::SendDelayedReply, reply_msg));
}
