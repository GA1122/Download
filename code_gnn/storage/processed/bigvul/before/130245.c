DataObjectImpl::DataObjectImpl()
    : is_aborting_(false),
      in_drag_loop_(false),
      in_async_mode_(false),
      async_operation_started_(false),
      observer_(NULL) {
}
