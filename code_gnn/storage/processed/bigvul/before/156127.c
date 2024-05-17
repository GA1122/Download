void HTMLLinkElement::ScheduleEvent() {
  GetDocument()
      .GetTaskRunner(TaskType::kDOMManipulation)
      ->PostTask(FROM_HERE,
                 WTF::Bind(&HTMLLinkElement::DispatchPendingEvent,
                           WrapPersistent(this),
                           WTF::Passed(IncrementLoadEventDelayCount::Create(
                               GetDocument()))));
}
