  void MaybeDestroyIBusConfig() {
    if (!ibus_) {
      LOG(ERROR) << "MaybeDestroyIBusConfig: ibus_ is NULL";
      return;
    }
    if (ibus_config_ && !ibus_bus_is_connected(ibus_)) {
      g_object_unref(ibus_config_);
      ibus_config_ = NULL;
     }
   }
