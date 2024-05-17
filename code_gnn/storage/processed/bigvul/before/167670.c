CSSStyleSheetResource* CSSStyleSheetResource::Fetch(FetchParameters& params,
                                                    ResourceFetcher* fetcher,
                                                    ResourceClient* client) {
  DCHECK_EQ(params.GetResourceRequest().GetFrameType(),
            network::mojom::RequestContextFrameType::kNone);
  params.SetRequestContext(mojom::RequestContextType::STYLE);
  CSSStyleSheetResource* resource = ToCSSStyleSheetResource(
      fetcher->RequestResource(params, CSSStyleSheetResourceFactory(), client));
  return resource;
}
