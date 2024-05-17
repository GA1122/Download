TabStripModel::~TabStripModel() {
  FOR_EACH_OBSERVER(TabStripModelObserver, observers_,
                    TabStripModelDeleted());
  STLDeleteContainerPointers(contents_data_.begin(), contents_data_.end());
  delete order_controller_;
}
