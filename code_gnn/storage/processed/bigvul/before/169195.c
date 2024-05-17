void DOMMessageQueue::ClearQueue() {
  message_queue_ = base::queue<std::string>();
}
