scoped_ptr<OverscrollRefresh> CreateRefreshEffect(
    OverscrollRefreshHandler* handler) {
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kDisablePullToRefreshEffect)) {
    return nullptr;
  }

  return make_scoped_ptr(new OverscrollRefresh(handler));
}
