WindowTreeHostManager::WindowTreeHostManager()
    : primary_tree_host_for_replace_(nullptr),
      focus_activation_store_(new FocusActivationStore()),
      cursor_window_controller_(new CursorWindowController()),
      mirror_window_controller_(new MirrorWindowController()),
      cursor_display_id_for_restore_(display::Display::kInvalidDisplayID),
      weak_ptr_factory_(this) {
  primary_display_id = display::Display::kInvalidDisplayID;
}
