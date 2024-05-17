MojoResult Core::ArmWatcher(MojoHandle watcher_handle,
                            uint32_t* num_ready_contexts,
                            uintptr_t* ready_contexts,
                            MojoResult* ready_results,
                            MojoHandleSignalsState* ready_signals_states) {
  RequestContext request_context;
  scoped_refptr<Dispatcher> watcher = GetDispatcher(watcher_handle);
  if (!watcher || watcher->GetType() != Dispatcher::Type::WATCHER)
    return MOJO_RESULT_INVALID_ARGUMENT;
  return watcher->Arm(num_ready_contexts, ready_contexts, ready_results,
                      ready_signals_states);
}
