void FetchManager::Loader::LoadSucceeded() {
  DCHECK(!failed_);

  finished_ = true;

  if (GetDocument() && GetDocument()->GetFrame() &&
      GetDocument()->GetFrame()->GetPage() &&
      CORS::IsOkStatus(response_http_status_code_)) {
    GetDocument()->GetFrame()->GetPage()->GetChromeClient().AjaxSucceeded(
        GetDocument()->GetFrame());
  }
  probe::didFinishFetch(execution_context_, this, fetch_request_data_->Method(),
                        fetch_request_data_->Url().GetString());
  NotifyFinished();
}
