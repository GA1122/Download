void TrimT(StringType* str) {
  str->erase(str->find_last_not_of(GetWhiteSpacesForType<StringType>()) + 1);
  str->erase(0, str->find_first_not_of(GetWhiteSpacesForType<StringType>()));
}
