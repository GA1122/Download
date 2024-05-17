std::unique_ptr<blink::URLLoaderFactoryBundleInfo> CloneFactoryBundle(
    scoped_refptr<blink::URLLoaderFactoryBundle> bundle) {
  return base::WrapUnique(static_cast<blink::URLLoaderFactoryBundleInfo*>(
      bundle->Clone().release()));
}
