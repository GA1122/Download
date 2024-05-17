void InFlightBackendIO::DoomEntriesBetween(const base::Time initial_time,
                        const base::Time end_time,
                        const net::CompletionCallback& callback) {
  scoped_refptr<BackendIO> operation(new BackendIO(this, backend_, callback));
  operation->DoomEntriesBetween(initial_time, end_time);
  PostOperation(FROM_HERE, operation.get());
}
