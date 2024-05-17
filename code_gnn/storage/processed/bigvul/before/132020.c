void ClipboardMessageFilter::OnReadImage(ui::ClipboardType type,
                                         IPC::Message* reply_msg) {
  SkBitmap bitmap = GetClipboard()->ReadImage(type);

#if defined(USE_X11)
  BrowserThread::PostTask(
      BrowserThread::FILE, FROM_HERE,
      base::Bind(
          &ClipboardMessageFilter::OnReadImageReply, this, bitmap, reply_msg));
#else
  OnReadImageReply(bitmap, reply_msg);
#endif
}
