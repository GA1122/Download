  DEFINE_INLINE_TRACE() {
    visitor->Trace(execution_context_);
    BaseFetchContext::Trace(visitor);
  }
