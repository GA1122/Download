FetchContext& FetchContext::NullInstance() {
  DEFINE_STATIC_LOCAL(FetchContext, instance, (new FetchContext));
  return instance;
}
