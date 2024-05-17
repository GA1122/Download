void RenderViewImpl::OnCustomContextMenuAction(
    const content::CustomContextMenuContext& custom_context,
    unsigned action) {
  if (custom_context.is_pepper_menu)
    pepper_delegate_.OnCustomContextMenuAction(custom_context, action);
  else
    webview()->performCustomContextMenuAction(action);
  FOR_EACH_OBSERVER(RenderViewObserver, observers_,
                    ContextMenuAction(action));
}
