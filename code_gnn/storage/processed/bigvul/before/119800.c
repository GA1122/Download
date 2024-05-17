void NavigationControllerImpl::NotifyEntryChanged(const NavigationEntry* entry,
                                                  int index) {
  EntryChangedDetails det;
  det.changed_entry = entry;
  det.index = index;
  NotificationService::current()->Notify(
      NOTIFICATION_NAV_ENTRY_CHANGED,
      Source<NavigationController>(this),
      Details<EntryChangedDetails>(&det));
}