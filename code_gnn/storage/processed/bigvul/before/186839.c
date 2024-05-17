 void RenderProcessHostImpl::CreateSharedRendererHistogramAllocator() {
   if (!base::GlobalHistogramAllocator::Get()) {
     if (is_initialized_) {
       HistogramController::GetInstance()->SetHistogramMemory<RenderProcessHost>(
           this, mojo::ScopedSharedBufferHandle());
     }
     return;
   }
 
   base::ProcessHandle destination = GetHandle();
   if (destination == base::kNullProcessHandle)
     return;
 
   if (!metrics_allocator_) {
     std::unique_ptr<base::SharedMemory> shm(new base::SharedMemory());
     if (!shm->CreateAndMapAnonymous(2 << 20))   
       return;
     metrics_allocator_.reset(new base::SharedPersistentMemoryAllocator(
         std::move(shm), GetID(), "RendererMetrics",  false));
   }
 
    HistogramController::GetInstance()->SetHistogramMemory<RenderProcessHost>(
        this, mojo::WrapSharedMemoryHandle(
                  metrics_allocator_->shared_memory()->handle().Duplicate(),
                metrics_allocator_->shared_memory()->mapped_size(), false));
//                 metrics_allocator_->shared_memory()->mapped_size(),
//                 mojo::UnwrappedSharedMemoryHandleProtection::kReadWrite));
  }