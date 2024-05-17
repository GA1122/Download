void WindowTreeHostManager::SetPrimaryDisplayId(int64_t id) {
  DCHECK_NE(display::Display::kInvalidDisplayID, id);
  if (id == display::Display::kInvalidDisplayID || primary_display_id == id ||
      window_tree_hosts_.size() < 2) {
    return;
  }
  if (GetDisplayManager()->GetNumDisplays() > 2)
    return;

  const display::Display& new_primary_display =
      GetDisplayManager()->GetDisplayForId(id);
  if (!new_primary_display.is_valid()) {
    LOG(ERROR) << "Invalid or non-existent display is requested:"
               << new_primary_display.ToString();
    return;
  }

  DisplayManager* display_manager = GetDisplayManager();
  DCHECK(new_primary_display.is_valid());
  DCHECK(display_manager->GetDisplayForId(new_primary_display.id()).is_valid());

  AshWindowTreeHost* non_primary_host =
      window_tree_hosts_[new_primary_display.id()];
  LOG_IF(ERROR, !non_primary_host)
      << "Unknown display is requested in SetPrimaryDisplay: id="
      << new_primary_display.id();
  if (!non_primary_host)
    return;

  display::Display old_primary_display =
      display::Screen::GetScreen()->GetPrimaryDisplay();
  DCHECK_EQ(old_primary_display.id(), primary_display_id);

  AshWindowTreeHost* primary_host = window_tree_hosts_[primary_display_id];
  CHECK(primary_host);
  CHECK_NE(primary_host, non_primary_host);

  window_tree_hosts_[new_primary_display.id()] = primary_host;
  GetRootWindowSettings(GetWindow(primary_host))->display_id =
      new_primary_display.id();

  window_tree_hosts_[old_primary_display.id()] = non_primary_host;
  GetRootWindowSettings(GetWindow(non_primary_host))->display_id =
      old_primary_display.id();

  const display::DisplayLayout& layout =
      GetDisplayManager()->GetCurrentDisplayLayout();
  if (layout.primary_id != new_primary_display.id()) {
    std::unique_ptr<display::DisplayLayout> swapped_layout(layout.Copy());
    swapped_layout->placement_list[0].Swap();
    swapped_layout->primary_id = new_primary_display.id();
    display::DisplayIdList list = display_manager->GetCurrentDisplayIdList();
    GetDisplayManager()->layout_store()->RegisterLayoutForDisplayIdList(
        list, std::move(swapped_layout));
  }

  primary_display_id = new_primary_display.id();

  UpdateWorkAreaOfDisplayNearestWindow(GetWindow(primary_host),
                                       old_primary_display.GetWorkAreaInsets());
  UpdateWorkAreaOfDisplayNearestWindow(GetWindow(non_primary_host),
                                       new_primary_display.GetWorkAreaInsets());

  GetDisplayManager()->set_force_bounds_changed(true);
  GetDisplayManager()->UpdateDisplays();
  GetDisplayManager()->set_force_bounds_changed(false);
}
