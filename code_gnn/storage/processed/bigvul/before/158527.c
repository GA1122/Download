void WebLocalFrameImpl::DispatchAfterPrintEvent() {
#if DCHECK_IS_ON()
  DCHECK(is_in_printing_) << "DispatchBeforePrintEvent() should be called "
                             "before DispatchAfterPrintEvent().";
  is_in_printing_ = false;
#endif

  if (View())
    DispatchPrintEventRecursively(EventTypeNames::afterprint);
}
