WebSocket::~WebSocket() {
  DCHECK(ready_state_ == INITIALIZED || !delegate_);
  DCHECK(!socket_stream_);
  DCHECK(!delegate_);
}
