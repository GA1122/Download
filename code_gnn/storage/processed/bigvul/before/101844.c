void Browser::TabRestoreServiceDestroyed(TabRestoreService* service) {
  if (!tab_restore_service_)
    return;

  DCHECK_EQ(tab_restore_service_, service);
  tab_restore_service_->RemoveObserver(this);
  tab_restore_service_ = NULL;
}
