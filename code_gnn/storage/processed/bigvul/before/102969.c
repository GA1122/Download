void TabStripModel::GetIndicesWithSameOpener(int index,
                                             std::vector<int>* indices) {
  NavigationController* opener = contents_data_[index]->group;
  if (!opener) {
    opener = &(GetTabContentsAt(index)->controller());
    if (!opener)
      return;
  }
  for (int i = 0; i < count(); ++i) {
    if (i == index)
      continue;
    if (contents_data_[i]->group == opener ||
        &(GetTabContentsAt(i)->controller()) == opener) {
      indices->push_back(i);
    }
  }
}
