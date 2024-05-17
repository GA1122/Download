void ResourceMessageFilter::DoOnGetRootWindowRect(gfx::NativeViewId view,
                                                  IPC::Message* reply_msg) {
  gfx::Rect rect;
  XID window;

  if (Singleton<GtkNativeViewManager>()->GetXIDForId(&window, view)) {
    if (window) {
      const XID toplevel = GetTopLevelWindow(window);
      int x, y;
      unsigned width, height;
      if (x11_util::GetWindowGeometry(&x, &y, &width, &height, toplevel))
        rect = gfx::Rect(x, y, width, height);
    }
  }

  ViewHostMsg_GetRootWindowRect::WriteReplyParams(reply_msg, rect);

   ChromeThread::PostTask(
      ChromeThread::IO, FROM_HERE,
      NewRunnableMethod(
          this, &ResourceMessageFilter::SendDelayedReply, reply_msg));
}
