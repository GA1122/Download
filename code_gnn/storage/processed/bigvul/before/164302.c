void PageCaptureSaveAsMHTMLFunction::TemporaryFileCreatedOnUI(bool success) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  if (!success) {
    ReturnFailure(kTemporaryFileError);
    return;
  }

  if (test_delegate_)
    test_delegate_->OnTemporaryFileCreated(mhtml_path_);

  WebContents* web_contents = GetWebContents();
  if (!web_contents) {
    ReturnFailure(kTabClosedError);
    return;
  }

  web_contents->GenerateMHTML(
      content::MHTMLGenerationParams(mhtml_path_),
      base::BindOnce(&PageCaptureSaveAsMHTMLFunction::MHTMLGenerated, this));
}
