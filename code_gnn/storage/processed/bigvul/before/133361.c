int64_t WindowTreeHostManager::GetPrimaryDisplayId() {
  CHECK_NE(display::Display::kInvalidDisplayID, primary_display_id);
  return primary_display_id;
}
