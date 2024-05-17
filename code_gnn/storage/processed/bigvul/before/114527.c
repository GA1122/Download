  ~MessageFilter() {
    for (ModalDialogEventMap::iterator i = modal_dialog_event_map_.begin();
        i != modal_dialog_event_map_.end(); ++i) {
      delete i->second.event;
    }
  }
