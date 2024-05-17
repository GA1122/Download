void SocketStream::OnIOCompleted(int result) {
  DoLoop(result);
}
