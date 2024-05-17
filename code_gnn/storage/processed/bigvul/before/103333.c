void ClipboardMessageFilter::OverrideThreadForMessage(
    const IPC::Message& message, BrowserThread::ID* thread) {
#if defined(OS_WIN)
  if (message.type() == ClipboardHostMsg_ReadImage::ID)
    *thread = BrowserThread::FILE;
#elif defined(USE_X11)
  if (IPC_MESSAGE_CLASS(message) == ClipboardMsgStart)
    *thread = BrowserThread::UI;
#endif
}
