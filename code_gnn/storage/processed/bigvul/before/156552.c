bool IsMalformedBlobUrl(const GURL& url) {
  if (!url.SchemeIsBlob())
    return false;

  std::string canonical_origin = url::Origin::Create(url).Serialize();
  canonical_origin.append(1, '/');
  if (base::StartsWith(url.GetContent(), canonical_origin,
                       base::CompareCase::INSENSITIVE_ASCII))
    return false;

  return true;
}
