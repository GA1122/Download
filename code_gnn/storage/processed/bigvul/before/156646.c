blink::ParsedFeaturePolicy CreateFPHeader(
    blink::mojom::FeaturePolicyFeature feature,
    const std::vector<GURL>& origins) {
  blink::ParsedFeaturePolicy result(1);
  result[0].feature = feature;
  result[0].matches_all_origins = false;
  DCHECK(!origins.empty());
  for (const GURL& origin : origins)
    result[0].origins.push_back(url::Origin::Create(origin));
  return result;
}
