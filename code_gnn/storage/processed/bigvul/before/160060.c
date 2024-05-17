  explicit IteratorImpl(base::WeakPtr<InFlightBackendIO> background_queue)
      : background_queue_(background_queue),
        iterator_(new Rankings::Iterator()) {
  }
