void TestExtensionSystem::CreateSocketManager() {
  BrowserThread::ID id;
  CHECK(BrowserThread::GetCurrentThreadIdentifier(&id));
  socket_manager_.reset(new ApiResourceManager<Socket>(id));
}
