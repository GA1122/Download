  bool Peek(UChar& character) const {
    if (position_ + 1 >= length_)
      return false;
    character = source_string_[position_ + 1];
    return true;
  }
