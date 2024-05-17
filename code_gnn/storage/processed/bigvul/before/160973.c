bool ChromeClientImpl::OpenBeforeUnloadConfirmPanelDelegate(LocalFrame* frame,
                                                            bool is_reload) {
  NotifyPopupOpeningObservers();
  WebLocalFrameImpl* webframe = WebLocalFrameImpl::FromFrame(frame);
  return webframe->Client() &&
         webframe->Client()->RunModalBeforeUnloadDialog(is_reload);
}
