bool MainThreadFrameObserver::OnMessageReceived(const IPC::Message& msg) {
  if (msg.type() == ViewHostMsg_WaitForNextFrameForTests_ACK::ID &&
      msg.routing_id() == routing_id_) {
    BrowserThread::PostTask(
        BrowserThread::UI, FROM_HERE,
        base::BindOnce(&MainThreadFrameObserver::Quit, base::Unretained(this)));
  }
  return true;
}
