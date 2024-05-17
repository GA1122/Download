MojoResult Core::CancelWatch(MojoHandle watcher_handle, uintptr_t context) {
  RequestContext request_context;
  scoped_refptr<Dispatcher> watcher = GetDispatcher(watcher_handle);
  if (!watcher || watcher->GetType() != Dispatcher::Type::WATCHER)
    return MOJO_RESULT_INVALID_ARGUMENT;
  return watcher->CancelWatch(context);
}
