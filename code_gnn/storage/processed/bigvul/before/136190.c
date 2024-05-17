inline uint32_t ReadUInt16(uint8_t* mem_bytes, int offset) {
  return
      static_cast<uint32_t>(mem_bytes[offset]) |
      (static_cast<uint32_t>(mem_bytes[offset + 1]) << 8);
}
