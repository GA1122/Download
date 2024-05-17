void Dispatcher::DispatchEvent(const std::string& extension_id,
                               const std::string& event_name) const {
  base::ListValue args;
  args.Set(0, new base::StringValue(event_name));
  args.Set(1, new base::ListValue());

  const char* local_event_bindings = kEventBindings;
  script_context_set_->ForEach(
      extension_id, base::Bind(&CallModuleMethod, local_event_bindings,
                               kEventDispatchFunction, &args));
}
