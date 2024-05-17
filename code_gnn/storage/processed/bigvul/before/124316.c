void TestExtensionSystem::CreateExtensionProcessManager() {
  extension_process_manager_.reset(ExtensionProcessManager::Create(profile_));
}
