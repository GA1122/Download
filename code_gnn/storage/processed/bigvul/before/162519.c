void Resource::AllClientsAndObserversRemoved() {
  if (loader_ && !detachable_)
    loader_->ScheduleCancel();
}
