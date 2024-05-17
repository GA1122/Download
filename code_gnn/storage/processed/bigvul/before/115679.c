bool HostNPScriptObject::Enumerate(std::vector<std::string>* values) {
  VLOG(2) << "Enumerate";
  CHECK_EQ(base::PlatformThread::CurrentId(), np_thread_id_);
  const char* entries[] = {
    kAttrNameAccessCode,
    kAttrNameState,
    kAttrNameLogDebugInfo,
    kAttrNameOnStateChanged,
    kFuncNameConnect,
    kFuncNameDisconnect,
    kFuncNameLocalize,
    kAttrNameDisconnected,
    kAttrNameStarting,
    kAttrNameRequestedAccessCode,
    kAttrNameReceivedAccessCode,
    kAttrNameConnected,
    kAttrNameDisconnecting,
    kAttrNameError
  };
  for (size_t i = 0; i < arraysize(entries); ++i) {
    values->push_back(entries[i]);
  }
  return true;
}
