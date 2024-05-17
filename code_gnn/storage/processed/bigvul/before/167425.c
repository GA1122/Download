void HTMLStyleElement::NotifyLoadedSheetAndAllCriticalSubresources(
    LoadedSheetErrorStatus error_status) {
  bool is_load_event = error_status == kNoErrorLoadingSubresource;
  if (fired_load_ && is_load_event)
    return;
  loaded_sheet_ = is_load_event;
  GetDocument()
      .GetTaskRunner(TaskType::kDOMManipulation)
      ->PostTask(FROM_HERE,
                 WTF::Bind(&HTMLStyleElement::DispatchPendingEvent,
                           WrapPersistent(this),
                           WTF::Passed(IncrementLoadEventDelayCount::Create(
                               GetDocument()))));
  fired_load_ = true;
}
