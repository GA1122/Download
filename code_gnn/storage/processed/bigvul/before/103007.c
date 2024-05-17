TabStripModel::TabStripModel(TabStripModelDelegate* delegate, Profile* profile)
    : delegate_(delegate),
      profile_(profile),
      closing_all_(false),
      order_controller_(NULL) {
  DCHECK(delegate_);
  registrar_.Add(this,
                 NotificationType::TAB_CONTENTS_DESTROYED,
                 NotificationService::AllSources());
  registrar_.Add(this,
                 NotificationType::EXTENSION_UNLOADED,
                 Source<Profile>(profile_));
  order_controller_ = new TabStripModelOrderController(this);
}
