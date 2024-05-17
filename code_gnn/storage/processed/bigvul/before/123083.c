base::i18n::TextDirection ConvertTextDirection(WebKit::WebTextDirection dir) {
  switch (dir) {
    case WebKit::WebTextDirectionDefault: return base::i18n::UNKNOWN_DIRECTION;
    case WebKit::WebTextDirectionLeftToRight: return base::i18n::LEFT_TO_RIGHT;
    case WebKit::WebTextDirectionRightToLeft: return base::i18n::RIGHT_TO_LEFT;
  }
  NOTREACHED() << "Unsupported text direction " << dir;
  return base::i18n::UNKNOWN_DIRECTION;
}
