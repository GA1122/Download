void Plugin::EnqueueProgressEvent(const char* event_type,
                                  const nacl::string& url,
                                  LengthComputable length_computable,
                                  uint64_t loaded_bytes,
                                  uint64_t total_bytes) {
  PLUGIN_PRINTF(("Plugin::EnqueueProgressEvent ("
                 "event_type='%s', url='%s', length_computable=%d, "
                 "loaded=%"NACL_PRIu64", total=%"NACL_PRIu64")\n",
                 event_type,
                 url.c_str(),
                 static_cast<int>(length_computable),
                 loaded_bytes,
                 total_bytes));

  progress_events_.push(new ProgressEvent(event_type,
                                          url,
                                          length_computable,
                                          loaded_bytes,
                                          total_bytes));
  pp::CompletionCallback callback =
      callback_factory_.NewCallback(&Plugin::DispatchProgressEvent);
  pp::Core* core = pp::Module::Get()->core();
  core->CallOnMainThread(0, callback, 0);
}
