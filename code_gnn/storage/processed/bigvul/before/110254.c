bool NaClIPCAdapter::SendCompleteMessage(const char* buffer,
                                         size_t buffer_len) {
  const NaClMessageHeader* header =
      reinterpret_cast<const NaClMessageHeader*>(buffer);

  int body_len = static_cast<int>(buffer_len - sizeof(NaClMessageHeader));
  DCHECK(body_len == static_cast<int>(header->payload_size));

  scoped_ptr<IPC::Message> msg(
      new IPC::Message(header->routing, header->type,
                       IPC::Message::PRIORITY_NORMAL));
  if (header->flags & IPC::Message::SYNC_BIT)
    msg->set_sync();
  if (header->flags & IPC::Message::REPLY_BIT)
    msg->set_reply();
  if (header->flags & IPC::Message::REPLY_ERROR_BIT)
    msg->set_reply_error();
  if (header->flags & IPC::Message::UNBLOCK_BIT)
    msg->set_unblock(true);

  msg->WriteBytes(&buffer[sizeof(NaClMessageHeader)], body_len);

  lock_.AssertAcquired();
  if (locked_data_.channel_closed_)
    return false;   

  task_runner_->PostTask(FROM_HERE,
      base::Bind(&NaClIPCAdapter::SendMessageOnIOThread, this,
                 base::Passed(&msg)));
  return true;
}
