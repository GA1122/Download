bool CreatePairImpl(HANDLE* socket_a, HANDLE* socket_b, bool overlapped) {
  DCHECK_NE(socket_a, socket_b);
  DCHECK_EQ(*socket_a, SyncSocket::kInvalidHandle);
  DCHECK_EQ(*socket_b, SyncSocket::kInvalidHandle);

  wchar_t name[kPipePathMax];
  ScopedHandle handle_a;
  DWORD flags = PIPE_ACCESS_DUPLEX | FILE_FLAG_FIRST_PIPE_INSTANCE;
  if (overlapped)
    flags |= FILE_FLAG_OVERLAPPED;

  do {
    unsigned int rnd_name;
    if (rand_s(&rnd_name) != 0)
      return false;

    swprintf(name, kPipePathMax,
             kPipeNameFormat,
             GetCurrentProcessId(),
             GetCurrentThreadId(),
             rnd_name);

    handle_a.Set(CreateNamedPipeW(
        name,
        flags,
        PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,
        1,
        kOutBufferSize,
        kInBufferSize,
        kDefaultTimeoutMilliSeconds,
        NULL));
  } while (!handle_a.IsValid() &&
           (GetLastError() == ERROR_PIPE_BUSY));

  if (!handle_a.IsValid()) {
    NOTREACHED();
    return false;
  }

  flags = SECURITY_SQOS_PRESENT | SECURITY_ANONYMOUS;
  if (overlapped)
    flags |= FILE_FLAG_OVERLAPPED;

  ScopedHandle handle_b(CreateFileW(name,
                                    GENERIC_READ | GENERIC_WRITE,
                                    0,           
                                    NULL,        
                                    OPEN_EXISTING,   
                                    flags,
                                    NULL));      
  if (!handle_b.IsValid()) {
    DPLOG(ERROR) << "CreateFileW failed";
    return false;
  }

  if (!ConnectNamedPipe(handle_a.Get(), NULL)) {
    DWORD error = GetLastError();
    if (error != ERROR_PIPE_CONNECTED) {
      DPLOG(ERROR) << "ConnectNamedPipe failed";
      return false;
    }
  }

  *socket_a = handle_a.Take();
  *socket_b = handle_b.Take();

  return true;
}
