bool Browser::ShouldSaveWindowPlacement() const {
  return (type() & TYPE_POPUP) == 0 || browser_defaults::kRestorePopups;
}
