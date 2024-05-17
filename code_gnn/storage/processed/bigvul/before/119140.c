size_t SyncSocket::Peek() {
  DWORD available = 0;
  PeekNamedPipe(handle_, NULL, 0, NULL, &available, NULL);
  return available;
}
