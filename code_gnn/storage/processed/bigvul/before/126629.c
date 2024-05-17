void TabStripModel::GetIndicesWithSameDomain(int index,
                                             std::vector<int>* indices) {
  std::string domain = GetWebContentsAt(index)->GetURL().host();
  if (domain.empty())
    return;
  for (int i = 0; i < count(); ++i) {
    if (i == index)
      continue;
    if (GetWebContentsAt(i)->GetURL().host() == domain)
      indices->push_back(i);
  }
}
