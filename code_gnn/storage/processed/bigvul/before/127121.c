bool SharedMemory::Lock(uint32 timeout_ms, SECURITY_ATTRIBUTES* sec_attr) {
  if (lock_ == NULL) {
    std::wstring name = name_;
    name.append(L"lock");
    lock_ = CreateMutex(sec_attr, FALSE, name.c_str());
    if (lock_ == NULL) {
      DPLOG(ERROR) << "Could not create mutex.";
      return false;   
    }
  }
  DWORD result = WaitForSingleObject(lock_, timeout_ms);

  return (result == WAIT_OBJECT_0);
}
