bool HostNPScriptObject::RemoveProperty(const std::string& property_name) {
  VLOG(2) << "RemoveProperty " << property_name;
  CHECK_EQ(base::PlatformThread::CurrentId(), np_thread_id_);
  return false;
}
