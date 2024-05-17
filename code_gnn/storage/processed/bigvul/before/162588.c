static bool TypeNeedsSynchronousCacheHit(Resource::Type type) {
  if (type == Resource::kCSSStyleSheet)
    return true;
  if (type == Resource::kScript)
    return true;
  if (type == Resource::kFont)
    return true;
  return false;
}
