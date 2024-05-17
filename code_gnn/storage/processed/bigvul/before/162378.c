MojoHandle Core::AddDispatcher(scoped_refptr<Dispatcher> dispatcher) {
  base::AutoLock lock(handles_->GetLock());
  return handles_->AddDispatcher(dispatcher);
}
