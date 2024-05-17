void InspectorHandler::TargetDetached(const std::string& reason) {
  frontend_->Detached(reason);
}
