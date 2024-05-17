static uint16_t ReadLE16(const uint8_t *data, size_t data_size,
 uint32_t read_offset) {
 if (read_offset + 1 > data_size)
 return 0;
 uint16_t val;
    val = data[read_offset];
    val |= data[read_offset + 1] << 8;
 return val;
}
