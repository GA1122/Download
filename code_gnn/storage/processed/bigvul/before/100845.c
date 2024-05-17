void HttpUtil::ParseContentType(const string& content_type_str,
                                string* mime_type, string* charset,
                                bool *had_charset) {
  size_t type_val = content_type_str.find_first_not_of(HTTP_LWS);
  type_val = std::min(type_val, content_type_str.length());
  size_t type_end = content_type_str.find_first_of(HTTP_LWS ";(", type_val);
  if (string::npos == type_end)
    type_end = content_type_str.length();

  size_t charset_val = 0;
  size_t charset_end = 0;

  bool type_has_charset = false;
  size_t param_start = content_type_str.find_first_of(';', type_end);
  if (param_start != string::npos) {
    size_t cur_param_start = param_start + 1;
    do {
      size_t cur_param_end =
          FindDelimiter(content_type_str, cur_param_start, ';');

      size_t param_name_start = content_type_str.find_first_not_of(
          HTTP_LWS, cur_param_start);
      param_name_start = std::min(param_name_start, cur_param_end);

      static const char charset_str[] = "charset=";
      size_t charset_end_offset = std::min(
          param_name_start + sizeof(charset_str) - 1, cur_param_end);
      if (LowerCaseEqualsASCII(
              content_type_str.begin() + param_name_start,
              content_type_str.begin() + charset_end_offset, charset_str)) {
        charset_val = param_name_start + sizeof(charset_str) - 1;
        charset_end = cur_param_end;
        type_has_charset = true;
      }

      cur_param_start = cur_param_end + 1;
    } while (cur_param_start < content_type_str.length());
  }

  if (type_has_charset) {
    charset_val = content_type_str.find_first_not_of(HTTP_LWS, charset_val);
    charset_val = std::min(charset_val, charset_end);
    char first_char = content_type_str[charset_val];
    if (first_char == '"' || first_char == '\'') {
      charset_end = FindStringEnd(content_type_str, charset_val, first_char);
      ++charset_val;
      DCHECK(charset_end >= charset_val);
    } else {
      charset_end = std::min(content_type_str.find_first_of(HTTP_LWS ";(",
                                                            charset_val),
                             charset_end);
    }
  }

  if (content_type_str.length() != 0 &&
      content_type_str != "*/*" &&
      content_type_str.find_first_of('/') != string::npos) {
    bool eq = !mime_type->empty() &&
              LowerCaseEqualsASCII(content_type_str.begin() + type_val,
                                   content_type_str.begin() + type_end,
                                   mime_type->data());
    if (!eq) {
      mime_type->assign(content_type_str.begin() + type_val,
                        content_type_str.begin() + type_end);
      StringToLowerASCII(mime_type);
    }
    if ((!eq && *had_charset) || type_has_charset) {
      *had_charset = true;
      charset->assign(content_type_str.begin() + charset_val,
                      content_type_str.begin() + charset_end);
      StringToLowerASCII(charset);
    }
  }
}
