  DEFINE_INLINE_VIRTUAL_TRACE() {
    visitor->Trace(plugin_);
    ChromePrintContext::Trace(visitor);
  }
