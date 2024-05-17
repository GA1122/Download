void Dispatcher::OnRenderProcessShutdown() {
  v8_schema_registry_.reset();
  forced_idle_timer_.reset();
  content_watcher_.reset();
  script_context_set_->ForEach(
      std::string(), nullptr,
      base::Bind(&ScriptContextSet::Remove,
                 base::Unretained(script_context_set_.get())));
}
