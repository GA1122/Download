bool DefaultTabHandler::RunUnloadListenerBeforeClosing(
    TabContentsWrapper* contents) {
   return delegate_->AsBrowser()->RunUnloadListenerBeforeClosing(contents);
 }
