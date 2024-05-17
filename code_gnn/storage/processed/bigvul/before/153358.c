void Tab::UpdateIconVisibility() {

  if (!closing_) {
    center_icon_ = false;
    extra_padding_before_content_ = false;
  }

  showing_icon_ = showing_alert_indicator_ = false;
  extra_alert_indicator_padding_ = false;

  if (height() < GetLayoutConstant(TAB_HEIGHT))
    return;

  const bool has_favicon = data().show_icon;
  const bool has_alert_icon =
      (alert_indicator_ ? alert_indicator_->showing_alert_state()
                        : data().alert_state) != TabAlertState::NONE;

  if (data().pinned) {
    showing_alert_indicator_ = has_alert_icon;
    showing_icon_ = has_favicon && !has_alert_icon;
    showing_close_button_ = false;
    return;
  }

  int available_width = GetContentsBounds().width();

  const bool touch_ui = MD::touch_ui();
  const int favicon_width = gfx::kFaviconSize;
  const int alert_icon_width = alert_indicator_->GetPreferredSize().width();
  const int close_button_width =
      close_button_->GetPreferredSize().width() -
      (touch_ui ? close_button_->GetInsets().right()
                : close_button_->GetInsets().width());
  const bool large_enough_for_close_button =
      available_width >= (touch_ui ? kTouchMinimumContentsWidthForCloseButtons
                                   : kMinimumContentsWidthForCloseButtons);

  showing_close_button_ = !controller_->ShouldHideCloseButtonForTab(this);
  if (IsActive()) {
    if (showing_close_button_)
      available_width -= close_button_width;

    showing_alert_indicator_ =
        has_alert_icon && alert_icon_width <= available_width;
    if (showing_alert_indicator_)
      available_width -= alert_icon_width;

    showing_icon_ = has_favicon && favicon_width <= available_width;
    if (showing_icon_)
      available_width -= favicon_width;
  } else {
    showing_alert_indicator_ =
        has_alert_icon && alert_icon_width <= available_width;
    if (showing_alert_indicator_)
      available_width -= alert_icon_width;

    showing_icon_ = has_favicon && favicon_width <= available_width;
    if (showing_icon_)
      available_width -= favicon_width;

    showing_close_button_ &= large_enough_for_close_button;
    if (showing_close_button_)
      available_width -= close_button_width;

    if (!showing_close_button_ && !showing_alert_indicator_ && !showing_icon_) {
      showing_alert_indicator_ = has_alert_icon;
      showing_icon_ = !showing_alert_indicator_ && has_favicon;

      if (!closing_)
        center_icon_ = true;
    }
  }

  if (!closing_) {
    extra_padding_before_content_ = large_enough_for_close_button;
  }

  extra_alert_indicator_padding_ = showing_alert_indicator_ &&
                                   showing_close_button_ &&
                                   large_enough_for_close_button;
}
