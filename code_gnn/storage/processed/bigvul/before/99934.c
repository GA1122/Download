bool PluginHost::SetPostData(const char* buf,
                             uint32 length,
                             std::vector<std::string>* names,
                             std::vector<std::string>* values,
                             std::vector<char>* body) {

  enum { INPUT_COLON=0, INPUT_NEWLINE, INPUT_NULL, INPUT_OTHER };
  enum { GETNAME, GETVALUE, GETDATA, DONE, ERR };
  int statemachine[3][4] = { { GETVALUE, GETDATA, GETDATA, GETNAME },
                             { GETVALUE, GETNAME, DONE, GETVALUE },
                             { GETDATA,  GETDATA, DONE, GETDATA } };
  std::string name, value;
  const char* ptr = static_cast<const char*>(buf);
  const char* start = ptr;
  int state = GETNAME;   
  bool done = false;
  bool err = false;
  do {
    int input;

    switch (*ptr) {
      case ':' :
        input = INPUT_COLON;
        break;
      case '\n':
        input = INPUT_NEWLINE;
        break;
      case 0   :
        input = INPUT_NULL;
        break;
      default  :
        input = INPUT_OTHER;
        break;
    }

    int newstate = statemachine[state][input];

    if (state != newstate) {
      switch (newstate) {
        case GETNAME:
          value = std::string(start, ptr - start);
          TrimWhitespace(value, TRIM_ALL, &value);
          if (!name.empty() && name != "content-length") {
            names->push_back(name);
            values->push_back(value);
          }
          start = ptr + 1;
          break;
        case GETVALUE:
          name = StringToLowerASCII(std::string(start, ptr - start));
          TrimWhitespace(name, TRIM_ALL, &name);
          start = ptr + 1;
          break;
        case GETDATA: {
          if (*ptr)
            start = ptr + 1;
          size_t previous_size = body->size();
          size_t new_body_size = length - static_cast<int>(start - buf);
          body->resize(previous_size + new_body_size);
          if (!body->empty())
            memcpy(&body->front() + previous_size, start, new_body_size);
          done = true;
          break;
        }
        case ERR:
          err = true;
          done = true;
          break;
      }
    }
    state = newstate;
    ptr++;
  } while (!done);

  return !err;
}
