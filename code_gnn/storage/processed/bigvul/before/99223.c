void ResourceMessageFilter::DoOnGetScreenInfo(gfx::NativeViewId view,
                                              IPC::Message* reply_msg) {
  Display* display = x11_util::GetSecondaryDisplay();
  int screen = x11_util::GetDefaultScreen(display);
  WebScreenInfo results = WebScreenInfoFactory::screenInfo(display, screen);
  ViewHostMsg_GetScreenInfo::WriteReplyParams(reply_msg, results);

  ChromeThread::PostTask(
      ChromeThread::IO, FROM_HERE,
      NewRunnableMethod(
          this, &ResourceMessageFilter::SendDelayedReply, reply_msg));
}
