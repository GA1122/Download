  void ReleaseModalDialogEvent(gfx::NativeViewId containing_window) {
    base::AutoLock auto_lock(modal_dialog_event_map_lock_);
    if (!modal_dialog_event_map_.count(containing_window)) {
      NOTREACHED();
      return;
    }

    if (--(modal_dialog_event_map_[containing_window].refcount))
      return;

    MessageLoop::current()->DeleteSoon(
        FROM_HERE, modal_dialog_event_map_[containing_window].event);
    modal_dialog_event_map_.erase(containing_window);
  }
