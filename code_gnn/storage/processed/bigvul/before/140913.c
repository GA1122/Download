void PresentationConnection::dispatchStateChangeEvent(Event* event) {
  TaskRunnerHelper::get(TaskType::Presentation, getExecutionContext())
      ->postTask(BLINK_FROM_HERE,
                 WTF::bind(&PresentationConnection::dispatchEventAsync,
                           wrapPersistent(this), wrapPersistent(event)));
}
