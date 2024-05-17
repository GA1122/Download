HTMLLinkElement::GetLoadingTaskRunner() {
  return GetDocument().GetTaskRunner(TaskType::kNetworking);
}
