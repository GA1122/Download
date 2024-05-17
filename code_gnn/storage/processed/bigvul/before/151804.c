void BackgroundLoaderOffliner::RunRenovations() {
  if (page_renovator_) {
    page_renovator_->RunRenovations(
        base::Bind(&BackgroundLoaderOffliner::RenovationsCompleted,
                   weak_ptr_factory_.GetWeakPtr()));
  }
}
