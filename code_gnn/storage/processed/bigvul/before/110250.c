 bool NaClIPCAdapter::OnMessageReceived(const IPC::Message& message) {
   {
     base::AutoLock lock(lock_);

    NaClMessageHeader header;
    memset(&header, 0, sizeof(NaClMessageHeader));

    header.payload_size = static_cast<uint32>(message.payload_size());
    header.routing = message.routing_id();
    header.type = message.type();
    header.flags = message.flags();
    header.num_fds = 0;   

    scoped_refptr<RewrittenMessage> dest(new RewrittenMessage);
    dest->SetData(header, message.payload(), message.payload_size());
    locked_data_.to_be_received_.push(dest);
  }
  cond_var_.Signal();
  return true;
}
