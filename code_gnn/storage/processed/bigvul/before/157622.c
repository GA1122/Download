  SimpleGetHelperResult SimpleGetHelper(base::span<const MockRead> data_reads) {
    MockWrite data_writes[] = {
        MockWrite("GET / HTTP/1.1\r\n"
                  "Host: www.example.org\r\n"
                  "Connection: keep-alive\r\n\r\n"),
    };

    StaticSocketDataProvider reads(data_reads, data_writes);
    StaticSocketDataProvider* data[] = {&reads};
    SimpleGetHelperResult out = SimpleGetHelperForData(data);

    EXPECT_EQ(CountWriteBytes(data_writes), out.total_sent_bytes);
    return out;
  }
