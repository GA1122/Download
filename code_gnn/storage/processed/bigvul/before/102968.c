void TabStripModel::GetIndicesWithSameDomain(int index,
                                             std::vector<int>* indices) {
  TabContentsWrapper* tab = GetTabContentsAt(index);
  std::string domain = tab->tab_contents()->GetURL().host();
  if (domain.empty())
    return;
  for (int i = 0; i < count(); ++i) {
    if (i == index)
      continue;
    if (GetTabContentsAt(i)->tab_contents()->GetURL().host() == domain)
      indices->push_back(i);
  }
}
