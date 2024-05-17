void DatabaseMessageFilter::OverrideThreadForMessage(
    const IPC::Message& message,
    BrowserThread::ID* thread) {
  if (message.type() == DatabaseHostMsg_GetSpaceAvailable::ID)
    *thread = BrowserThread::IO;
  else if (IPC_MESSAGE_CLASS(message) == DatabaseMsgStart)
    *thread = BrowserThread::FILE;

  if (message.type() == DatabaseHostMsg_Opened::ID && !observer_added_) {
    observer_added_ = true;
    BrowserThread::PostTask(
        BrowserThread::FILE, FROM_HERE,
        base::Bind(&DatabaseMessageFilter::AddObserver, this));
  }
}
