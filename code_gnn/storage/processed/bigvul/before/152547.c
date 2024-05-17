FaviconURL::IconType ToFaviconType(blink::WebIconURL::Type type) {
  switch (type) {
    case blink::WebIconURL::kTypeFavicon:
      return FaviconURL::IconType::kFavicon;
    case blink::WebIconURL::kTypeTouch:
      return FaviconURL::IconType::kTouchIcon;
    case blink::WebIconURL::kTypeTouchPrecomposed:
      return FaviconURL::IconType::kTouchPrecomposedIcon;
    case blink::WebIconURL::kTypeInvalid:
      return FaviconURL::IconType::kInvalid;
  }
  NOTREACHED();
  return FaviconURL::IconType::kInvalid;
}
