  void OnNotifyStreamCreated(
      int stream_id,
      base::SharedMemoryHandle handle,
      base::SyncSocket::TransitDescriptor socket_descriptor,
      uint32_t length) {
    shared_memory_.reset(new base::SharedMemory(handle, false));
    CHECK(shared_memory_->Map(length));
    CHECK(shared_memory_->memory());
    shared_memory_length_ = length;

    base::SyncSocket::Handle sync_socket_handle =
        base::SyncSocket::UnwrapHandle(socket_descriptor);
    sync_socket_.reset(new base::SyncSocket(sync_socket_handle));

    WasNotifiedOfCreation(stream_id, length);
  }
