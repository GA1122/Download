bool SetKeyboardEventText(blink::WebUChar* to, Maybe<std::string> from) {
  if (!from.isJust())
    return true;

  base::string16 text16 = base::UTF8ToUTF16(from.fromJust());
  if (text16.size() > blink::WebKeyboardEvent::kTextLengthCap)
    return false;

  for (size_t i = 0; i < text16.size(); ++i)
    to[i] = text16[i];
  return true;
}
