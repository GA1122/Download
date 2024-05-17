void WebLocalFrameImpl::DidCallAddSearchProvider() {
  UseCounter::Count(GetFrame(), WebFeature::kExternalAddSearchProvider);
}
