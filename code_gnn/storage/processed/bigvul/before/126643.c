bool TabStripModel::IsTabDiscarded(int index) const {
  return contents_data_[index]->discarded;
}
