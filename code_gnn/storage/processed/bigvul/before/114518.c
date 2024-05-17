  void OnInit(const PluginMsg_Init_Params& params, IPC::Message* reply_msg) {
    base::AutoLock auto_lock(modal_dialog_event_map_lock_);
    if (modal_dialog_event_map_.count(params.containing_window)) {
      modal_dialog_event_map_[params.containing_window].refcount++;
      return;
    }

    WaitableEventWrapper wrapper;
    wrapper.event = new base::WaitableEvent(true, false);
    wrapper.refcount = 1;
    modal_dialog_event_map_[params.containing_window] = wrapper;
  }
