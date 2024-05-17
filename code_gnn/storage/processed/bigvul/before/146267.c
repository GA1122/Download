  UChar Current() {
    SECURITY_DCHECK(position_ < length_);
    return source_string_[position_];
  }
