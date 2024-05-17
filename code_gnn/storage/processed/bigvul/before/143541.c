  void TryEstablishVizConnectionIfNeeded() {
    if (!pending_connect_viz_on_io_thread_)
      return;
    base::PostTaskWithTraits(FROM_HERE, {BrowserThread::IO},
                             std::move(pending_connect_viz_on_io_thread_));
  }
