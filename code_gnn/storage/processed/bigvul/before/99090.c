net::LoadState MoreInterestingLoadState(net::LoadState a, net::LoadState b) {
  return (a < b) ? b : a;
}
