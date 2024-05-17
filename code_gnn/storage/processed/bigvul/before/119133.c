CancelableSyncSocket::CancelableSyncSocket(Handle handle)
    : SyncSocket(handle), shutdown_event_(true, false),
      file_operation_(true, false) {
}
