void AppCache::ToResourceInfoVector(
    std::vector<blink::mojom::AppCacheResourceInfo>* infos) const {
  DCHECK(infos && infos->empty());
  for (const auto& pair : entries_) {
    infos->push_back(blink::mojom::AppCacheResourceInfo());
    blink::mojom::AppCacheResourceInfo& info = infos->back();
    info.url = pair.first;
    info.is_master = pair.second.IsMaster();
    info.is_manifest = pair.second.IsManifest();
    info.is_intercept = pair.second.IsIntercept();
    info.is_fallback = pair.second.IsFallback();
    info.is_foreign = pair.second.IsForeign();
    info.is_explicit = pair.second.IsExplicit();
    info.size = pair.second.response_size();
    info.response_id = pair.second.response_id();
  }
}
