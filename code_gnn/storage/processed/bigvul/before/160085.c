int BackendImpl::RunTaskForTest(const base::Closure& task,
                                const CompletionCallback& callback) {
  background_queue_.RunTask(task, callback);
  return net::ERR_IO_PENDING;
}
