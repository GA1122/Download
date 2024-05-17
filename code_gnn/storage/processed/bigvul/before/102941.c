DefaultTabHandler::~DefaultTabHandler() {
  DCHECK(model_->empty());
  model_->RemoveObserver(this);
}
