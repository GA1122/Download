MojoResult Core::Close(MojoHandle handle) {
  RequestContext request_context;
  scoped_refptr<Dispatcher> dispatcher;
  {
    base::AutoLock lock(handles_->GetLock());
    MojoResult rv = handles_->GetAndRemoveDispatcher(handle, &dispatcher);
    if (rv != MOJO_RESULT_OK)
      return rv;
  }
  dispatcher->Close();
  return MOJO_RESULT_OK;
}
