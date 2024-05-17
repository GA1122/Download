void BrowserView::ToolbarSizeChanged(bool is_animating) {
  static CallState call_state = NORMAL;

  bool use_fast_resize =
      is_animating || (call_state == REENTRANT_FORCE_FAST_RESIZE);
  if (use_fast_resize)
    contents_container_->SetFastResize(true);
  UpdateUIForContents(GetActiveTabContents());
  if (use_fast_resize)
    contents_container_->SetFastResize(false);

  {
    int top_arrow_height = 0;
    if (!chrome::search::IsInstantExtendedAPIEnabled(browser()->profile()) ||
        !browser()->search_model()->mode().is_ntp()) {
      const LocationIconView* location_icon_view =
          toolbar_->location_bar()->location_icon_view();
      gfx::Point icon_bottom(0, location_icon_view->GetImageBounds().bottom() -
          LocationBarView::kIconInternalPadding + 1);
      ConvertPointToTarget(location_icon_view, this, &icon_bottom);
      gfx::Point infobar_top(0, infobar_container_->GetVerticalOverlap(NULL));
      ConvertPointToTarget(infobar_container_, this, &infobar_top);
      top_arrow_height = infobar_top.y() - icon_bottom.y();
    }
    AutoReset<CallState> resetter(&call_state,
        is_animating ? REENTRANT_FORCE_FAST_RESIZE : REENTRANT);
    infobar_container_->SetMaxTopArrowHeight(top_arrow_height);
  }

  if ((call_state == NORMAL) && !is_animating) {
    contents_container_->InvalidateLayout();
    contents_split_->Layout();
  }
}
