void InFlightBackendIO::PostOperation(const base::Location& from_here,
                                      BackendIO* operation) {
  background_thread_->PostTask(
      from_here, base::Bind(&BackendIO::ExecuteOperation, operation));
  OnOperationPosted(operation);
}
