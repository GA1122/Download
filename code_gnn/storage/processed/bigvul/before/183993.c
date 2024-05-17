   static void IBusBusConnectedCallback(IBusBus* bus, gpointer user_data) {
//   void IBusBusConnected(IBusBus* bus) {
      LOG(WARNING) << "IBus connection is recovered.";
    g_return_if_fail(user_data);
    InputMethodStatusConnection* self
        = static_cast<InputMethodStatusConnection*>(user_data);
    self->MaybeRestoreConnections();
//     MaybeRestoreConnections();
    }