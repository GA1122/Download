void Core::RequestShutdown(const base::Closure& callback) {
  GetNodeController()->RequestShutdown(callback);
}
