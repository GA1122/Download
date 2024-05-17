void SetShelfAutoHideBehaviorPref(int64_t display_id,
                                  ShelfAutoHideBehavior behavior) {
  PrefService* prefs =
      Shell::Get()->session_controller()->GetLastActiveUserPrefService();
  if (!prefs)
    return;
  SetShelfAutoHideBehaviorPref(prefs, display_id, behavior);
}
