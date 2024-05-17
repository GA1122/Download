void InFlightBackendIO::RunTask(
    const base::Closure& task, const net::CompletionCallback& callback) {
  scoped_refptr<BackendIO> operation(new BackendIO(this, backend_, callback));
  operation->RunTask(task);
  PostOperation(FROM_HERE, operation.get());
}
