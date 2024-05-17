  void LocalFileSystem::fileSystemNotAllowedInternal(
      PassRefPtrWillBeRawPtr<ExecutionContext> context,
    PassRefPtr<CallbackWrapper> callbacks)
//     CallbackWrapper* callbacks)
  {
      context->postTask(createCrossThreadTask(&reportFailure, callbacks->release(), FileError::ABORT_ERR));
  }