void DiscardableSharedMemoryManager::Bind(
    mojom::DiscardableSharedMemoryManagerRequest request,
    const service_manager::BindSourceInfo& source_info) {
  DCHECK(!mojo_thread_message_loop_ ||
         mojo_thread_message_loop_ == base::MessageLoop::current());
  if (!mojo_thread_message_loop_) {
    mojo_thread_message_loop_ = base::MessageLoop::current();
    mojo_thread_message_loop_->AddDestructionObserver(this);
  }

  mojo::MakeStrongBinding(
      std::make_unique<MojoDiscardableSharedMemoryManagerImpl>(
          next_client_id_++, mojo_thread_weak_ptr_factory_.GetWeakPtr()),
      std::move(request));
}
