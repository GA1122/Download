bool HasGoogleSearchQueryParam(base::StringPiece str) {
  url::Component query(0, static_cast<int>(str.length())), key, value;
  while (url::ExtractQueryKeyValue(str.data(), &query, &key, &value)) {
    base::StringPiece key_str = str.substr(key.begin, key.len);
    if (key_str == "q" || key_str == "as_q")
      return true;
  }
  return false;
}
