void ChromeWebContentsDelegateAndroid::LoadingStateChanged(
    WebContents* source, bool to_different_document) {
  bool has_stopped = source == NULL || !source->IsLoading();
  WebContentsDelegateAndroid::LoadingStateChanged(
      source, to_different_document);
  LoadProgressChanged(source, has_stopped ? 1 : 0);
}
