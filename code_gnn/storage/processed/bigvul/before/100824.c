bool HttpUtil::HeadersIterator::AdvanceTo(const char* name) {
  DCHECK(name != NULL);
  DCHECK_EQ(0, StringToLowerASCII<std::string>(name).compare(name))
      << "the header name must be in all lower case";

  while (GetNext()) {
    if (LowerCaseEqualsASCII(name_begin_, name_end_, name)) {
      return true;
    }
  }

  return false;
}
