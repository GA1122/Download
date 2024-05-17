void TabStrip::RemoveTabDelegate::CompleteRemove() {
  DCHECK(tab_->closing());
  tabstrip_->RemoveAndDeleteTab(tab_);
  HighlightCloseButton();
}
