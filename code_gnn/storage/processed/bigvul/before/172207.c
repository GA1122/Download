static void expect_data_multibyte(eager_reader_t *reader, void *context) {
 char *data = (char *)context;
 int length = strlen(data);

 for (int i = 0; i < length;) {
 uint8_t buffer[28];
 int bytes_to_read = (length - i) > 28 ? 28 : (length - i);
 int bytes_read = eager_reader_read(reader, buffer, bytes_to_read, false);
    EXPECT_EQ(bytes_to_read, bytes_read);
 for (int j = 0; j < bytes_read && i < length; j++, i++) {
      EXPECT_EQ(data[i], buffer[j]);
 }
 }

  semaphore_post(done);
}
