scoped_refptr<Dispatcher> Core::GetDispatcher(MojoHandle handle) {
  base::AutoLock lock(handles_->GetLock());
  return handles_->GetDispatcher(handle);
}
