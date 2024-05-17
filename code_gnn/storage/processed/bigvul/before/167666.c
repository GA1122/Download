CSSStyleSheetResource* CSSStyleSheetResource::CreateForTest(
    const KURL& url,
    const WTF::TextEncoding& encoding) {
  ResourceRequest request(url);
  request.SetFetchCredentialsMode(network::mojom::FetchCredentialsMode::kOmit);
  ResourceLoaderOptions options;
  TextResourceDecoderOptions decoder_options(
      TextResourceDecoderOptions::kCSSContent, encoding);
  return new CSSStyleSheetResource(request, options, decoder_options);
}
