void SVGStyleElement::NotifyLoadedSheetAndAllCriticalSubresources(
    LoadedSheetErrorStatus error_status) {
  if (error_status != kNoErrorLoadingSubresource) {
    GetDocument()
        .GetTaskRunner(TaskType::kDOMManipulation)
        ->PostTask(FROM_HERE, WTF::Bind(&SVGStyleElement::DispatchPendingEvent,
                                        WrapPersistent(this)));
  }
}
