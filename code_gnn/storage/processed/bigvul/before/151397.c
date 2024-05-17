void FrameFetchContext::CountUsage(WebFeature feature) const {
  if (IsDetached())
    return;
  UseCounter::Count(GetFrame(), feature);
}
