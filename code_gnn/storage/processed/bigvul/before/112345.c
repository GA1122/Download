const net::LoadStateWithParam& MoreInterestingLoadState(
    const net::LoadStateWithParam& a, const net::LoadStateWithParam& b) {
  return (a.state < b.state) ? b : a;
}
