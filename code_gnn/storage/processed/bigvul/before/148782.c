TextInputManager* InterstitialPageImpl::GetTextInputManager() {
  return !web_contents_ ? nullptr : static_cast<WebContentsImpl*>(web_contents_)
                                        ->GetTextInputManager();
}
