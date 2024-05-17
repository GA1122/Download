void BackgroundLoaderOffliner::OnPageSaved(SavePageResult save_result,
                                           int64_t offline_id) {
  if (!pending_request_)
    return;

  SavePageRequest request(*pending_request_.get());
  bool did_snapshot_on_last_retry = did_snapshot_on_last_retry_;
  ResetState();

  if (save_state_ == DELETE_AFTER_SAVE) {
    PageCriteria criteria;
    criteria.offline_ids = std::vector<int64_t>{offline_id};
    offline_page_model_->DeletePagesWithCriteria(
        criteria,
        base::Bind(&BackgroundLoaderOffliner::DeleteOfflinePageCallback,
                   weak_ptr_factory_.GetWeakPtr(), request));
    save_state_ = NONE;
    return;
  }

  save_state_ = NONE;

  Offliner::RequestStatus save_status;
  if (save_result == SavePageResult::ALREADY_EXISTS) {
    save_status = RequestStatus::SAVED;
  } else if (save_result == SavePageResult::SUCCESS) {
    if (did_snapshot_on_last_retry)
      save_status = RequestStatus::SAVED_ON_LAST_RETRY;
    else
      save_status = RequestStatus::SAVED;
  } else {
    save_status = RequestStatus::SAVE_FAILED;
  }

  std::move(completion_callback_).Run(request, save_status);
}
