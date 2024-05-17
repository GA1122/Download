void OmniboxViewViews::OnMouseReleased(const ui::MouseEvent& event) {
  views::Textfield::OnMouseReleased(event);
  if ((event.IsOnlyLeftMouseButton() || event.IsOnlyRightMouseButton()) &&
      select_all_on_mouse_release_) {
    SelectAllForUserGesture();
    if (location_bar_view_)
      location_bar_view_->omnibox_page_action_icon_container_view()
          ->UpdatePageActionIcon(PageActionIconType::kSendTabToSelf);
  }
  select_all_on_mouse_release_ = false;

  is_mouse_pressed_ = false;
  filter_drag_events_for_unelision_ = false;

  if (UnapplySteadyStateElisions(UnelisionGesture::MOUSE_RELEASE))
    TextChanged();
}
