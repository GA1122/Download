    void SimulateOnBufferCreated(int buffer_id, const base::SharedMemory& shm) {
    auto handle = base::SharedMemory::DuplicateHandle(shm.handle());
      video_capture_impl_->OnBufferCreated(
        buffer_id, mojo::WrapSharedMemoryHandle(handle, shm.mapped_size(),
                                                true  ));
//         buffer_id, mojo::WrapSharedMemoryHandle(
//                        shm.GetReadOnlyHandle(), shm.mapped_size(),
//                        mojo::UnwrappedSharedMemoryHandleProtection::kReadOnly));
    }