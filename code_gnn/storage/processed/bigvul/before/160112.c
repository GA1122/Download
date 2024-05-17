BackendIO::BackendIO(InFlightIO* controller, BackendImpl* backend,
                     const net::CompletionCallback& callback)
    : BackgroundIO(controller),
      backend_(backend),
      callback_(callback),
      operation_(OP_NONE),
      entry_ptr_(NULL),
      iterator_(NULL),
      entry_(NULL),
      index_(0),
      offset_(0),
      buf_len_(0),
      truncate_(false),
      offset64_(0),
      start_(NULL) {
  start_time_ = base::TimeTicks::Now();
}
