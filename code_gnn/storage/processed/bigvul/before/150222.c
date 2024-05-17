void TabletModeWindowManager::SetIgnoreWmEventsForExit() {
  for (auto& pair : window_state_map_)
    pair.second->set_ignore_wm_events(true);
}
