TabContentsWrapper* PrintPreviewMessageHandler::tab_contents_wrapper() {
  return TabContentsWrapper::GetCurrentWrapperForContents(web_contents());
}
