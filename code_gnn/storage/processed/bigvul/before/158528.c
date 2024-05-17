void WebLocalFrameImpl::DispatchBeforePrintEvent() {
#if DCHECK_IS_ON()
  DCHECK(!is_in_printing_) << "DispatchAfterPrintEvent() should have been "
                              "called after the previous "
                              "DispatchBeforePrintEvent() call.";
  is_in_printing_ = true;
#endif

  DispatchPrintEventRecursively(EventTypeNames::beforeprint);
}
