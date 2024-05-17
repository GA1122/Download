void ChromeClientImpl::EnumerateChosenDirectory(FileChooser* file_chooser) {
  WebViewClient* client = web_view_->Client();
  if (!client)
    return;

  WebFileChooserCompletionImpl* chooser_completion =
      new WebFileChooserCompletionImpl(file_chooser);

  DCHECK(file_chooser);
  DCHECK(file_chooser->Params().selected_files.size());

  if (!client->EnumerateChosenDirectory(
          file_chooser->Params().selected_files[0], chooser_completion))
    chooser_completion->DidChooseFile(WebVector<WebString>());
}
