bool IndexedDBDispatcher::Send(IPC::Message* msg) {
  if (CurrentWorkerId()) {
    scoped_refptr<IPC::SyncMessageFilter> filter(
        ChildThread::current()->sync_message_filter());
    return filter->Send(msg);
  }
  return ChildThread::current()->Send(msg);
}
