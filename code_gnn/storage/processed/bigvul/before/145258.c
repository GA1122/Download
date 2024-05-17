void Dispatcher::UpdateBindings(const std::string& extension_id) {
  script_context_set().ForEach(extension_id,
                               base::Bind(&Dispatcher::UpdateBindingsForContext,
                                          base::Unretained(this)));
}
