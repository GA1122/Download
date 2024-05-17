void Core::PassNodeControllerToIOThread(
    std::unique_ptr<NodeController> node_controller) {
  node_controller.release()->DestroyOnIOThreadShutdown();
}
