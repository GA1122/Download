void WebLocalFrameImpl::DidCallIsSearchProviderInstalled() {
  UseCounter::Count(GetFrame(), WebFeature::kExternalIsSearchProviderInstalled);
}
