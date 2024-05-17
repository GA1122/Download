HICON HWNDMessageHandler::GetDefaultWindowIcon() const {
  if (use_system_default_icon_)
    return NULL;
  return ViewsDelegate::views_delegate ?
      ViewsDelegate::views_delegate->GetDefaultWindowIcon() : NULL;
}
