 void MojoVideoEncodeAccelerator::UseOutputBitstreamBuffer(
     const BitstreamBuffer& buffer) {
   DVLOG(2) << __func__ << " buffer.id()= " << buffer.id()
             << " buffer.size()= " << buffer.size() << "B";
    DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  
// 
//    
//    
    mojo::ScopedSharedBufferHandle buffer_handle = mojo::WrapSharedMemoryHandle(
      buffer.handle().Duplicate(), buffer.size(), true  );
//       buffer.handle().Duplicate(), buffer.size(),
//       mojo::UnwrappedSharedMemoryHandleProtection::kReadWrite);
  
    vea_->UseOutputBitstreamBuffer(buffer.id(), std::move(buffer_handle));
  }