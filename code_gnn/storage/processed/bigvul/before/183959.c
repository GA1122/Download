   static void RegisterPropertiesHandler(
      void* object, const ImePropertyList& prop_list) {
//    
//   virtual void OnRegisterImeProperties(
//       const input_method::ImePropertyList& prop_list) {
      if (!BrowserThread::CurrentlyOn(BrowserThread::UI)) {
        LOG(ERROR) << "Not on UI thread";
        return;
      }
  
    InputMethodLibraryImpl* input_method_library =
        static_cast<InputMethodLibraryImpl*>(object);
    input_method_library->RegisterProperties(prop_list);
//     RegisterProperties(prop_list);
    }