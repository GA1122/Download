void ChromeClientImpl::OpenFileChooser(LocalFrame* frame,
                                       RefPtr<FileChooser> file_chooser) {
  NotifyPopupOpeningObservers();
  WebFrameClient* client = WebLocalFrameImpl::FromFrame(frame)->Client();
  if (!client)
    return;

  const WebFileChooserParams& params = file_chooser->Params();
  WebFileChooserCompletionImpl* chooser_completion =
      new WebFileChooserCompletionImpl(std::move(file_chooser));
  if (client->RunFileChooser(params, chooser_completion))
    return;
  chooser_completion->DidChooseFile(WebVector<WebString>());
}
