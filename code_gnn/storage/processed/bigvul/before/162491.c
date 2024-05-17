ScriptResource* ScriptResource::Fetch(FetchParameters& params,
                                      ResourceFetcher* fetcher,
                                      ResourceClient* client) {
  DCHECK_EQ(params.GetResourceRequest().GetFrameType(),
            network::mojom::RequestContextFrameType::kNone);
  params.SetRequestContext(WebURLRequest::kRequestContextScript);
  return ToScriptResource(
      fetcher->RequestResource(params, ScriptResourceFactory(), client));
}
