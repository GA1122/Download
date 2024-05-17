  void ShutDownOnIOThread() {
    DCHECK(io_thread_checker_.CalledOnValidThread());

    weak_factory_.InvalidateWeakPtrs();

    if (message_loop_observer_) {
      message_loop_observer_->ShutDown();
      message_loop_observer_ = nullptr;
    }

    scoped_refptr<IOThreadContext> keepalive(this);

    factory_bindings_.CloseAllBindings();
    service_context_.reset();

    ClearConnectionFiltersOnIOThread();

    request_handlers_.clear();
    embedded_services_.clear();
    child_binding_.Close();
  }
