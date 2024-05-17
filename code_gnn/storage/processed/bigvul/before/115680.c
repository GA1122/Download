bool HostNPScriptObject::HasMethod(const std::string& method_name) {
  VLOG(2) << "HasMethod " << method_name;
  CHECK_EQ(base::PlatformThread::CurrentId(), np_thread_id_);
  return (method_name == kFuncNameConnect ||
          method_name == kFuncNameDisconnect ||
          method_name == kFuncNameLocalize);
}
