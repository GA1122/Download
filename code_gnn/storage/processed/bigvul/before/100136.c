void BrowserActionButton::UpdateState() {
  int tab_id = panel_->GetCurrentTabId();
  if (tab_id < 0)
    return;

  SkBitmap image = browser_action()->GetIcon(tab_id);
  if (!image.isNull())
    SetIcon(image);
  else if (!default_icon_.isNull())
    SetIcon(default_icon_);

  SetTooltipText(UTF8ToWide(browser_action()->GetTitle(tab_id)));
  GetParent()->SchedulePaint();
}
