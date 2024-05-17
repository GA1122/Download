bool BrowserView::DrawInfoBarArrows(int* x) const {
  if (x) {
    const LocationIconView* location_icon_view =
        toolbar_->location_bar()->location_icon_view();
    gfx::Point icon_center(location_icon_view->GetImageBounds().CenterPoint());
    ConvertPointToTarget(location_icon_view, this, &icon_center);
    *x = icon_center.x();
  }
  return true;
}
