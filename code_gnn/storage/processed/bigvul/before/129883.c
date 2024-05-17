  void Upload(const scoped_refptr<base::RefCountedString>& file_contents,
              scoped_ptr<base::DictionaryValue> metadata,
              base::Callback<void()> done_callback) {
    receive_count_ += 1;
    EXPECT_TRUE(file_contents);

    size_t compressed_length = file_contents->data().length();
    const size_t kOutputBufferLength = 10 * 1024 * 1024;
    std::vector<char> output_str(kOutputBufferLength);

    z_stream stream = {0};
    stream.avail_in = compressed_length;
    stream.avail_out = kOutputBufferLength;
    stream.next_in = (Bytef*)&file_contents->data()[0];
    stream.next_out = (Bytef*)vector_as_array(&output_str);

    int result = inflateInit2(&stream, 16 + MAX_WBITS);
    EXPECT_EQ(Z_OK, result);
    result = inflate(&stream, Z_FINISH);
    int bytes_written = kOutputBufferLength - stream.avail_out;

    inflateEnd(&stream);
    EXPECT_EQ(Z_STREAM_END, result);

    last_file_contents_.assign(vector_as_array(&output_str), bytes_written);
    BrowserThread::PostTask(BrowserThread::UI, FROM_HERE,
                            base::Bind(done_callback));
    BrowserThread::PostTask(BrowserThread::UI, FROM_HERE,
                            base::Bind(callback_));
  }
