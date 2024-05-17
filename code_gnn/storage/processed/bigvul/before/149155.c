bool IDNSpoofChecker::IsMadeOfLatinAlikeCyrillic(
    const icu::UnicodeString& label) {
  icu::UnicodeSet cyrillic_in_label;
  icu::StringCharacterIterator it(label);
  for (it.setToStart(); it.hasNext();) {
    const UChar32 c = it.next32PostInc();
    if (cyrillic_letters_.contains(c))
      cyrillic_in_label.add(c);
  }
  return !cyrillic_in_label.isEmpty() &&
         cyrillic_letters_latin_alike_.containsAll(cyrillic_in_label);
}
