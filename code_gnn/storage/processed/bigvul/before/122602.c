void ExtensionFunctionDispatcher::OnExtensionFunctionCompleted(
    const Extension* extension) {
  extensions::ExtensionSystem::Get(profile())->process_manager()->
      DecrementLazyKeepaliveCount(extension);
}
