Response EmulationHandler::CanEmulate(bool* result) {
#if defined(OS_ANDROID)
  *result = false;
#else
  *result = true;
  if (WebContentsImpl* web_contents = GetWebContents())
    *result &= !web_contents->GetVisibleURL().SchemeIs(kChromeDevToolsScheme);
  if (host_ && host_->GetRenderWidgetHost())
    *result &= !host_->GetRenderWidgetHost()->auto_resize_enabled();
#endif   
  return Response::OK();
}
