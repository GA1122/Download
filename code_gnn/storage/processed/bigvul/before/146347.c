void StripComments::Process(UChar c) {
  if (IsNewline(c)) {
    Emit(c);

    if (parse_state_ != kInMultiLineComment)
      parse_state_ = kBeginningOfLine;

    return;
  }

  UChar temp = 0;
  switch (parse_state_) {
    case kBeginningOfLine:
      if (WTF::IsASCIISpace(c)) {
        Emit(c);
        break;
      }

      if (c == '#') {
        parse_state_ = kInPreprocessorDirective;
        Emit(c);
        break;
      }

      parse_state_ = kMiddleOfLine;
      Process(c);
      break;

    case kMiddleOfLine:
      if (c == '/' && Peek(temp)) {
        if (temp == '/') {
          parse_state_ = kInSingleLineComment;
          Emit(' ');
          Advance();
          break;
        }

        if (temp == '*') {
          parse_state_ = kInMultiLineComment;
          Emit('/');
          Emit('*');
          Advance();
          break;
        }
      }

      Emit(c);
      break;

    case kInPreprocessorDirective:
      Emit(c);
      break;

    case kInSingleLineComment:
      if (c == '\\') {
        if (Peek(temp) && IsNewline(temp))
          Advance();
      }

      break;

    case kInMultiLineComment:
      if (c == '*' && Peek(temp) && temp == '/') {
        Emit('*');
        Emit('/');
        parse_state_ = kMiddleOfLine;
        Advance();
        break;
      }

      break;
  }
}
