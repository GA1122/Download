void SocketStream::ContinueDespiteError() {
  base::MessageLoop::current()->PostTask(
      FROM_HERE, base::Bind(&SocketStream::DoLoop, this, OK));
}
