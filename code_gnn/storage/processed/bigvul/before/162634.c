void HeadlessDevToolsManagerDelegate::SetNetworkConditions(
    std::vector<HeadlessBrowserContext*> browser_contexts,
    HeadlessNetworkConditions conditions) {
  for (std::vector<HeadlessBrowserContext*>::iterator it =
           browser_contexts.begin();
       it != browser_contexts.end(); ++it) {
    HeadlessBrowserContextImpl* context =
        static_cast<HeadlessBrowserContextImpl*>(*it);
    context->SetNetworkConditions(conditions);
  }
}
