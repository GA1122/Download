static inline String CanonicalizedTitle(Document* document,
                                        const String& title) {
  unsigned length = title.length();
  unsigned builder_index = 0;
  const CharacterType* characters = title.GetCharacters<CharacterType>();

  StringBuffer<CharacterType> buffer(length);

  bool pending_whitespace = false;
  for (unsigned i = 0; i < length; ++i) {
    UChar32 c = characters[i];
    if ((c <= WTF::unicode::kSpaceCharacter &&
         c != WTF::unicode::kLineTabulationCharacter) ||
        c == WTF::unicode::kDeleteCharacter) {
      if (builder_index != 0)
        pending_whitespace = true;
    } else {
      if (pending_whitespace) {
        buffer[builder_index++] = ' ';
        pending_whitespace = false;
      }
      buffer[builder_index++] = c;
    }
  }
  buffer.Shrink(builder_index);

  return String::Adopt(buffer);
}
