MojoResult Core::QueryHandleSignalsState(
    MojoHandle handle,
    MojoHandleSignalsState* signals_state) {
  RequestContext request_context;
  scoped_refptr<Dispatcher> dispatcher = GetDispatcher(handle);
  if (!dispatcher || !signals_state)
    return MOJO_RESULT_INVALID_ARGUMENT;
  *signals_state = dispatcher->GetHandleSignalsState();
  return MOJO_RESULT_OK;
}
