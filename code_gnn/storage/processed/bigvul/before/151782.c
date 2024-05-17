void BackgroundLoaderOffliner::CanDownload(
    base::OnceCallback<void(bool)> callback) {
  if (!pending_request_.get()) {
    std::move(callback).Run(false);   
    return;
  }

  bool should_allow_downloads = false;
  Offliner::RequestStatus final_status =
      Offliner::RequestStatus::LOADING_FAILED_DOWNLOAD;
  if (offline_page_model_->GetPolicyController()
          ->AllowsConversionToBackgroundFileDownload(
              pending_request_.get()->client_id().name_space)) {
    should_allow_downloads = true;
    final_status = Offliner::RequestStatus::DOWNLOAD_THROTTLED;
  }

  std::move(callback).Run(should_allow_downloads);
  SavePageRequest request(*pending_request_.get());
  std::move(completion_callback_).Run(request, final_status);
  base::ThreadTaskRunnerHandle::Get()->PostTask(
      FROM_HERE, base::BindOnce(&BackgroundLoaderOffliner::ResetState,
                                weak_ptr_factory_.GetWeakPtr()));
}
