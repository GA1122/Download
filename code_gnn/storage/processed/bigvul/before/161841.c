  DiscardableMemoryImpl(ClientDiscardableSharedMemoryManager* manager,
                        std::unique_ptr<DiscardableSharedMemoryHeap::Span> span)
      : manager_(manager), span_(std::move(span)), is_locked_(true) {}
