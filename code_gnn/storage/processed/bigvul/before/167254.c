void FetchManager::Loader::DidFinishLoading(unsigned long, double) {
  did_finish_loading_ = true;
  if (integrity_verifier_ && !integrity_verifier_->IsFinished())
    return;

  LoadSucceeded();
}
