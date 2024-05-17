   bool ChangeInputMethod(const char* name) {
//    
//   virtual bool ChangeInputMethod(const std::string& name) {
      if (!IBusConnectionsAreAlive()) {
        LOG(ERROR) << "ChangeInputMethod: IBus connection is not alive";
        return false;
      }
    if (!name) {
//     if (name.empty()) {
        return false;
      }
      if (!InputMethodIdIsWhitelisted(name)) {
       LOG(ERROR) << "Input method '" << name << "' is not supported";
       return false;
     }
 
    RegisterProperties(NULL);
//     DoRegisterProperties(NULL);
  
      ibus_bus_set_global_engine_async(ibus_,
                                     name,
//                                      name.c_str(),
                                       -1,   
                                       NULL,   
                                       NULL,   
                                       NULL);   
      return true;
    }