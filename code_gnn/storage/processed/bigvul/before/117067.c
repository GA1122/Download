  SessionService::FindClosestNavigationWithIndex(
    std::vector<TabNavigation>* navigations,
    int index) {
  DCHECK(navigations);
  for (std::vector<TabNavigation>::iterator i = navigations->begin();
       i != navigations->end(); ++i) {
    if (i->index() >= index)
      return i;
  }
  return navigations->end();
}
