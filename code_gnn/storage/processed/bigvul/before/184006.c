   bool StopInputMethodProcess() {
//    
//   virtual bool StopInputMethodProcess() {
      if (!IBusConnectionsAreAlive()) {
        LOG(ERROR) << "StopInputMethodProcess: IBus connection is not alive";
        return false;
     }
 
     ibus_bus_exit_async(ibus_,
                         FALSE   ,
                         -1   ,
                         NULL   ,
                         NULL   ,
                         NULL   );
 
     if (ibus_config_) {
       g_object_unref(ibus_config_);
       ibus_config_ = NULL;
     }
      return true;
    }