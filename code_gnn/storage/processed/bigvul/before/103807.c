void RenderView::OnCustomContextMenuAction(
    const webkit_glue::CustomContextMenuContext& custom_context,
    unsigned action) {
  if (custom_context.is_pepper_menu)
    pepper_delegate_.OnCustomContextMenuAction(custom_context, action);
  else
    webview()->performCustomContextMenuAction(action);
}
