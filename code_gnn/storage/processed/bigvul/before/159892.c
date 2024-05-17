void TopSitesImpl::OnTopSitesAvailableFromHistory(
    const MostVisitedURLList* pages) {
  DCHECK(pages);
  SetTopSites(*pages, CALL_LOCATION_FROM_OTHER_PLACES);
}
