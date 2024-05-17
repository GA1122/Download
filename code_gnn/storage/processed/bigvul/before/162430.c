MojoResult Core::Watch(MojoHandle watcher_handle,
                       MojoHandle handle,
                       MojoHandleSignals signals,
                       MojoWatchCondition condition,
                       uintptr_t context) {
  RequestContext request_context;
  scoped_refptr<Dispatcher> watcher = GetDispatcher(watcher_handle);
  if (!watcher || watcher->GetType() != Dispatcher::Type::WATCHER)
    return MOJO_RESULT_INVALID_ARGUMENT;
  scoped_refptr<Dispatcher> dispatcher = GetDispatcher(handle);
  if (!dispatcher)
    return MOJO_RESULT_INVALID_ARGUMENT;
  return watcher->WatchDispatcher(std::move(dispatcher), signals, condition,
                                  context);
}
