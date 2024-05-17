void Launcher::CycleWindowLinear(CycleDirection direction) {
  int item_index = GetNextActivatedItemIndex(*model(), direction);
  if (item_index >= 0)
    ActivateLauncherItem(item_index);
}
