ssize_t NaClDescCustomSendMsg(void* handle, const NaClImcTypedMsgHdr* msg,
                              int  ) {
  if (msg->iov_length != 1)
    return -1;
  return static_cast<ssize_t>(
      ToAdapter(handle)->Send(static_cast<char*>(msg->iov[0].base),
                              msg->iov[0].length));
}
