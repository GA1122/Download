void TabStripModel::GetIndicesWithSameOpener(int index,
                                             std::vector<int>* indices) {
  WebContents* opener = contents_data_[index]->group;
  if (!opener) {
    opener = GetWebContentsAt(index);
    if (!opener)
      return;
  }
  for (int i = 0; i < count(); ++i) {
    if (i == index)
      continue;
    if (contents_data_[i]->group == opener ||
        GetWebContentsAtImpl(i) == opener) {
      indices->push_back(i);
    }
  }
}
