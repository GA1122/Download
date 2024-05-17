  static void PrepareData(scoped_ptr<CompoundBuffer>* buffer) {
    static const std::string kTestData =
        "Hello world!"
        "This is testing"
        "MultipleArrayInputStream"
        "for Chromoting";

    int segments = (kTestData.length() / 3) * 2;
    int remaining_chars = kTestData.length() % 3;
    if (remaining_chars) {
      if (remaining_chars == 1)
        ++segments;
      else
        segments += 2;
    }

    CompoundBuffer* result = new CompoundBuffer();
    const char* data = kTestData.data();
    for (int i = 0; i < segments; ++i) {
      int size = i % 2 == 0 ? 1 : 2;
      result->Append(new net::WrappedIOBuffer(data), size);
      data += size;
    }
    result->Lock();
    buffer->reset(result);
  }
