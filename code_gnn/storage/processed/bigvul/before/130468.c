  AsyncPixelTransferCompletionObserverImpl(base::subtle::Atomic32 submit_count)
      : submit_count_(submit_count), cancelled_(false) {}
