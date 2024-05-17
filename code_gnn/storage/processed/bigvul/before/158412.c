std::string GetMessageNames(
    const MockWidgetInputHandler::MessageVector& events) {
  std::vector<std::string> result;
  for (auto& event : events)
    result.push_back(event->name());
  return base::JoinString(result, " ");
}
