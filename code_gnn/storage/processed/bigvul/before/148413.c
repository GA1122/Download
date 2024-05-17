void WebContentsImpl::GetScreenInfo(ScreenInfo* screen_info) {
  if (GetView())
    GetView()->GetScreenInfo(screen_info);
}
