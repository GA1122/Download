  bool TriggerAutoHideTimeout() const {
    ShelfLayoutManager* layout_manager = GetShelfLayoutManager();
    if (!layout_manager->auto_hide_timer_.IsRunning())
      return false;

    layout_manager->auto_hide_timer_.FireNow();
    return true;
  }
