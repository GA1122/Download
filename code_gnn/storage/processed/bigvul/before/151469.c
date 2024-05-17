  void ExpectUpgrade(const char* input, const char* expected) {
    ExpectUpgrade(input, WebURLRequest::kRequestContextScript,
                  WebURLRequest::kFrameTypeNone, expected);
  }
