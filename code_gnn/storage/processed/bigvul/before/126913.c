bool BrowserTabStripController::IsTabPinned(BaseTab* tab) const {
  return IsTabPinned(tabstrip_->GetModelIndexOfBaseTab(tab));
}
