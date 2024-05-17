bool TabStripModel::IsTabBlocked(int index) const {
  return contents_data_[index]->blocked;
}
