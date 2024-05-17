AshWindowTreeHost* WindowTreeHostManager::GetAshWindowTreeHostForDisplayId(
    int64_t display_id) {
  CHECK_EQ(1u, window_tree_hosts_.count(display_id)) << "display id = "
                                                     << display_id;
  return window_tree_hosts_[display_id];
}
