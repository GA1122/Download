static inline UChar* AppendCharacter(UChar* destination, int character) {
  DCHECK(!IsNonCharacter(character));
  DCHECK(!U_IS_SURROGATE(character));
  if (U_IS_BMP(character)) {
    *destination++ = static_cast<UChar>(character);
  } else {
    *destination++ = U16_LEAD(character);
    *destination++ = U16_TRAIL(character);
  }
  return destination;
}
