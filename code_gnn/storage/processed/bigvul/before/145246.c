void Dispatcher::OnUnloaded(const std::string& id) {
  if (!RendererExtensionRegistry::Get()->Remove(id))
    return;

  active_extension_ids_.erase(id);

  script_injection_manager_->OnExtensionUnloaded(id);

  ScriptInjection::RemoveIsolatedWorld(id);

  std::set<ScriptContext*> removed_contexts =
      script_context_set_->OnExtensionUnloaded(id);
  for (ScriptContext* context : removed_contexts) {
    request_sender_->InvalidateSource(context);
  }

  UpdateBindings("");

  EraseL10nMessagesMap(id);

}
