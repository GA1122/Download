AsyncPixelTransfersCompletedQuery::~AsyncPixelTransfersCompletedQuery() {
  if (observer_)
    observer_->Cancel();
}
