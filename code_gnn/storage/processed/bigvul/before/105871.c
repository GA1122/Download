  static size_t ReadFromInput(CompoundBufferInputStream* input,
                              void* data, size_t size) {
    uint8* out = reinterpret_cast<uint8*>(data);
    int out_size = size;

    const void* in;
    int in_size = 0;

    while (true) {
      if (!input->Next(&in, &in_size)) {
        return size - out_size;
      }
      EXPECT_GT(in_size, -1);

      if (out_size <= in_size) {
        memcpy(out, in, out_size);
        if (in_size > out_size) {
          input->BackUp(in_size - out_size);
        }
        return size;   
      }

      memcpy(out, in, in_size);
      out += in_size;
      out_size -= in_size;
    }
  }
