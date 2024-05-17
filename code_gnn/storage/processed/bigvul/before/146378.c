  StripComments(const String& str)
      : parse_state_(kBeginningOfLine),
        source_string_(str),
        length_(str.length()),
        position_(0) {
    Parse();
  }
