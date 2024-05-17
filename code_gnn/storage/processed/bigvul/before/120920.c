void SocketStream::CancelWithError(int error) {
  base::MessageLoop::current()->PostTask(
      FROM_HERE, base::Bind(&SocketStream::DoLoop, this, error));
}
