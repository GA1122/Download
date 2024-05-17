void ExtensionWebContentsObserver::PepperInstanceDeleted() {
  ProcessManager* const process_manager = ProcessManager::Get(browser_context_);
  const Extension* const extension =
      process_manager->GetExtensionForWebContents(web_contents());
  if (extension)
    process_manager->DecrementLazyKeepaliveCount(extension);
}
