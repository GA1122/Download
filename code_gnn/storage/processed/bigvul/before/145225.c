bool Dispatcher::IsRuntimeAvailableToContext(ScriptContext* context) {
  for (const auto& extension :
       *RendererExtensionRegistry::Get()->GetMainThreadExtensionSet()) {
    ExternallyConnectableInfo* info = static_cast<ExternallyConnectableInfo*>(
        extension->GetManifestData(manifest_keys::kExternallyConnectable));
    if (info && info->matches.MatchesURL(context->url()))
      return true;
  }
  return false;
}
