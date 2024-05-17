void Tab::SetData(TabRendererData data) {
  DCHECK(GetWidget());

  if (data_ == data)
    return;

  TabRendererData old(std::move(data_));
  data_ = std::move(data);

  icon_->SetData(data_);
  icon_->SetCanPaintToLayer(controller_->CanPaintThrobberToLayer());
  UpdateTabIconNeedsAttentionBlocked();

  base::string16 title = data_.title;
  if (title.empty()) {
    title = icon_->ShowingLoadingAnimation()
                ? l10n_util::GetStringUTF16(IDS_TAB_LOADING_TITLE)
                : CoreTabHelper::GetDefaultTitle();
  } else {
    title = Browser::FormatTitleForDisplay(title);
  }
  title_->SetText(title);

  if (data_.alert_state != old.alert_state)
    alert_indicator_->TransitionToAlertState(data_.alert_state);
  if (old.pinned != data_.pinned)
    showing_alert_indicator_ = false;

  if (data_.alert_state != old.alert_state || data_.title != old.title)
    TooltipTextChanged();

  Layout();
  SchedulePaint();
}
