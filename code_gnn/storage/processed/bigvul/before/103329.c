void ClipboardMessageFilter::OnReadImage(
    ui::Clipboard::Buffer buffer, IPC::Message* reply_msg) {
  SkBitmap bitmap = GetClipboard()->ReadImage(buffer);

#if defined(USE_X11)
  BrowserThread::PostTask(
      BrowserThread::FILE, FROM_HERE,
      NewRunnableMethod(
          this, &ClipboardMessageFilter::OnReadImageReply, bitmap, reply_msg));
#else
  OnReadImageReply(bitmap, reply_msg);
#endif
 }
