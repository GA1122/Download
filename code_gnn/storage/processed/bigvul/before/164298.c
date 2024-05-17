void PageCaptureSaveAsMHTMLFunction::ReturnFailure(const std::string& error) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  error_ = error;

  SendResponse(false);

}
