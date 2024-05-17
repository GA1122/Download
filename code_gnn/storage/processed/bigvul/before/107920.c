void InfoBarContainer::Observe(NotificationType type,
                               const NotificationSource& source,
                               const NotificationDetails& details) {
  if (type == NotificationType::TAB_CONTENTS_INFOBAR_ADDED) {
    AddInfoBar(Details<InfoBarDelegate>(details).ptr(), true);   
  } else if (type == NotificationType::TAB_CONTENTS_INFOBAR_REMOVED) {
    RemoveInfoBar(Details<InfoBarDelegate>(details).ptr(), true);   
  } else if (type == NotificationType::TAB_CONTENTS_INFOBAR_REPLACED) {
    std::pair<InfoBarDelegate*, InfoBarDelegate*>* delegates =
        Details<std::pair<InfoBarDelegate*, InfoBarDelegate*> >(details).ptr();
    ReplaceInfoBar(delegates->first, delegates->second);
  } else {
    NOTREACHED();
  }
}
