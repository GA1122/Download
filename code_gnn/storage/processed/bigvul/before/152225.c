std::vector<gfx::Size> ConvertToFaviconSizes(
    const blink::WebVector<blink::WebSize>& web_sizes) {
  std::vector<gfx::Size> result;
  result.reserve(web_sizes.size());
  for (const blink::WebSize& web_size : web_sizes)
    result.push_back(gfx::Size(web_size));
  return result;
}
