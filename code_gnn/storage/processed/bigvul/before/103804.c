void RenderView::OnContextMenuClosed(
    const webkit_glue::CustomContextMenuContext& custom_context) {
  if (custom_context.is_pepper_menu)
    pepper_delegate_.OnContextMenuClosed(custom_context);
  else
    context_menu_node_.reset();
}
