void ResourceMessageFilter::DoOnClipboardIsFormatAvailable(
    Clipboard::FormatType format, Clipboard::Buffer buffer,
    IPC::Message* reply_msg) {
  const bool result = GetClipboard()->IsFormatAvailable(format, buffer);

  ViewHostMsg_ClipboardIsFormatAvailable::WriteReplyParams(reply_msg, result);

   ChromeThread::PostTask(
      ChromeThread::IO, FROM_HERE,
      NewRunnableMethod(
          this, &ResourceMessageFilter::SendDelayedReply, reply_msg));
}
