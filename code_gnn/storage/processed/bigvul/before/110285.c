void Plugin::DispatchProgressEvent(int32_t result) {
  PLUGIN_PRINTF(("Plugin::DispatchProgressEvent (result=%"
                 NACL_PRId32")\n", result));
  if (result < 0) {
    return;
  }
  if (progress_events_.empty()) {
    PLUGIN_PRINTF(("Plugin::DispatchProgressEvent: no pending events\n"));
    return;
  }
  nacl::scoped_ptr<ProgressEvent> event(progress_events_.front());
  progress_events_.pop();
  PLUGIN_PRINTF(("Plugin::DispatchProgressEvent ("
                 "event_type='%s', url='%s', length_computable=%d, "
                 "loaded=%"NACL_PRIu64", total=%"NACL_PRIu64")\n",
                 event->event_type(),
                 event->url(),
                 static_cast<int>(event->length_computable()),
                 event->loaded_bytes(),
                 event->total_bytes()));

  static const char* kEventClosureJS =
      "(function(target, type, url,"
      "          lengthComputable, loadedBytes, totalBytes) {"
      "    var progress_event = new ProgressEvent(type, {"
      "        bubbles: false,"
      "        cancelable: true,"
      "        lengthComputable: lengthComputable,"
      "        loaded: loadedBytes,"
      "        total: totalBytes"
      "      });"
      "    progress_event.url = url;"
      "    target.dispatchEvent(progress_event);"
      "})";

  pp::VarPrivate exception;
  pp::VarPrivate function_object = ExecuteScript(kEventClosureJS, &exception);
  if (!exception.is_undefined() || !function_object.is_object()) {
    PLUGIN_PRINTF(("Plugin::DispatchProgressEvent:"
                   " Function object creation failed.\n"));
    return;
  }
  pp::Var owner_element_object = GetOwnerElementObject();
  if (!owner_element_object.is_object()) {
    PLUGIN_PRINTF(("Plugin::DispatchProgressEvent:"
                   " Couldn't get owner element object.\n"));
    NACL_NOTREACHED();
    return;
  }

  pp::Var argv[6];
  static const uint32_t argc = NACL_ARRAY_SIZE(argv);
  argv[0] = owner_element_object;
  argv[1] = pp::Var(event->event_type());
  argv[2] = pp::Var(event->url());
  argv[3] = pp::Var(event->length_computable() == LENGTH_IS_COMPUTABLE);
  argv[4] = pp::Var(static_cast<double>(event->loaded_bytes()));
  argv[5] = pp::Var(static_cast<double>(event->total_bytes()));

  const pp::Var default_method;
  function_object.Call(default_method, argc, argv, &exception);
  if (!exception.is_undefined()) {
    PLUGIN_PRINTF(("Plugin::DispatchProgressEvent:"
                   " event dispatch failed.\n"));
  }
}
