static void expect_data(eager_reader_t *reader, void *context) {
 char *data = (char *)context;
 int length = strlen(data);

 for (int i = 0; i < length; i++) {
 uint8_t byte;
    EXPECT_EQ((size_t)1, eager_reader_read(reader, &byte, 1, true));
    EXPECT_EQ(data[i], byte);
 }

  semaphore_post(done);
}
