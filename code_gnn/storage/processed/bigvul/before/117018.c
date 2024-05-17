gfx::Rect OmniboxPopupViewGtk::GetTargetBounds() {
  if (!gtk_widget_get_realized(window_))
    return gfx::Rect();

  gfx::Rect retval = ui::GetWidgetScreenBounds(window_);

  GtkRequisition req;
  gtk_widget_size_request(window_, &req);
  retval.set_width(req.width);
  retval.set_height(req.height);

  return retval;
}
