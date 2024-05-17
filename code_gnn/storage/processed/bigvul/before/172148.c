static void expect_packet_synchronous(serial_data_type_t type, char *packet_data) {
 int length = strlen(packet_data);
 for (int i = 0; i < length; i++) {
 uint8_t byte;
    EXPECT_EQ((size_t)1, hal->read_data(type, &byte, 1, true));
    EXPECT_EQ(packet_data[i], byte);
 }

  hal->packet_finished(type);
}
