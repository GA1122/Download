void NavigationController::NotifyEntryChanged(const NavigationEntry* entry,
                                              int index) {
  content::EntryChangedDetails det;
  det.changed_entry = entry;
  det.index = index;
  content::NotificationService::current()->Notify(
      content::NOTIFICATION_NAV_ENTRY_CHANGED,
      content::Source<NavigationController>(this),
      content::Details<content::EntryChangedDetails>(&det));
}
