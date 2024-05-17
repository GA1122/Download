    LRUCanvasResourceProviderCache(int capacity)
    : resource_providers_(
          std::make_unique<std::unique_ptr<CanvasResourceProvider>[]>(
              capacity)),
      capacity_(capacity) {}
