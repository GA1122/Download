static void btsnoop_write_packet(packet_type_t type, const uint8_t *packet, bool is_received) {
 int length_he = 0;
 int length;
 int flags;
 int drops = 0;
 switch (type) {
 case kCommandPacket:
      length_he = packet[2] + 4;
      flags = 2;
 break;
 case kAclPacket:
      length_he = (packet[3] << 8) + packet[2] + 5;
      flags = is_received;
 break;
 case kScoPacket:
      length_he = packet[2] + 4;
      flags = is_received;
 break;
 case kEventPacket:
      length_he = packet[1] + 3;
      flags = 3;
 break;
 }

 uint64_t timestamp = btsnoop_timestamp();
 uint32_t time_hi = timestamp >> 32;
 uint32_t time_lo = timestamp & 0xFFFFFFFF;

  length = htonl(length_he);
  flags = htonl(flags);
  drops = htonl(drops);
  time_hi = htonl(time_hi);
  time_lo = htonl(time_lo);

  btsnoop_write(&length, 4);
  btsnoop_write(&length, 4);
  btsnoop_write(&flags, 4);
  btsnoop_write(&drops, 4);
  btsnoop_write(&time_hi, 4);
  btsnoop_write(&time_lo, 4);
  btsnoop_write(&type, 1);
  btsnoop_write(packet, length_he - 1);
}
