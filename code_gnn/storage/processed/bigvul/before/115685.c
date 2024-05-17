bool HostNPScriptObject::InvokeDefault(const NPVariant* args,
                                       uint32_t argCount,
                                       NPVariant* result) {
  VLOG(2) << "InvokeDefault";
  CHECK_EQ(base::PlatformThread::CurrentId(), np_thread_id_);
  SetException("exception during default invocation");
  return false;
}
