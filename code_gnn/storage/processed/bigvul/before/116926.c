void TabLoader::ScheduleLoad(NavigationController* controller) {
  DCHECK(controller);
  DCHECK(find(tabs_to_load_.begin(), tabs_to_load_.end(), controller) ==
         tabs_to_load_.end());
  tabs_to_load_.push_back(controller);
  RegisterForNotifications(controller);
}
