MojoResult Core::AcquireDispatchersForTransit(
    const MojoHandle* handles,
    size_t num_handles,
    std::vector<Dispatcher::DispatcherInTransit>* dispatchers) {
  base::AutoLock lock(handles_->GetLock());
  MojoResult rv = handles_->BeginTransit(handles, num_handles, dispatchers);
  if (rv != MOJO_RESULT_OK)
    handles_->CancelTransit(*dispatchers);
  return rv;
}
