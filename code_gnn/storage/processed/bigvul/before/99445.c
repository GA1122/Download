bool SocketWriteErrorIsRecoverable() {
#if defined(OS_MACOSX)
  return errno == EAGAIN || errno == EMSGSIZE;
#else
  return errno == EAGAIN;
#endif
}
