void ChromeClientImpl::SetCursor(const WebCursorInfo& cursor,
                                 LocalFrame* local_frame) {
  if (cursor_overridden_)
    return;

#if defined(OS_MACOSX)
  if (web_view_->HasOpenedPopup())
    return;
#endif

  LocalFrame& local_root = local_frame->LocalFrameRoot();
  if (WebFrameWidgetBase* widget =
          WebLocalFrameImpl::FromFrame(&local_root)->FrameWidget())
    widget->Client()->DidChangeCursor(cursor);
}
