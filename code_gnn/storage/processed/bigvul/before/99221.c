void ResourceMessageFilter::DoOnClipboardReadText(Clipboard::Buffer buffer,
                                                  IPC::Message* reply_msg) {
  string16 result;
  GetClipboard()->ReadText(buffer, &result);

  ViewHostMsg_ClipboardReadText::WriteReplyParams(reply_msg, result);

   ChromeThread::PostTask(
      ChromeThread::IO, FROM_HERE,
      NewRunnableMethod(
          this, &ResourceMessageFilter::SendDelayedReply, reply_msg));
}
