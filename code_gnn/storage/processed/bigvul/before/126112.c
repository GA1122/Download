TabCountChangeObserver::~TabCountChangeObserver() {
  tab_strip_model_->RemoveObserver(this);
}
