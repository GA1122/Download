static int set_name(int argc, char **argv) {
 if (argc != 1) {
    printf("Device name not specified.\n");
 return 1;
 }

 size_t len = strlen(argv[0]);
 if (len > 247) {
    printf("Device name cannot exceed 247 bytes.\n");
 return 2;
 }

 uint8_t packet[251] = { 0x13, 0x0C, 248 };
  memcpy(&packet[3], argv[0], len + 1);

 if (!write_hci_command(HCI_PACKET_COMMAND, packet, sizeof(packet)))
 return 1;

  memset(&packet[0], sizeof(packet), 0);
  packet[0] = 0x52;
  packet[1] = 0x0C;
  packet[2] = 0xF1;  
  packet[3] = 0x01;  
  packet[4] = len + 1;
  packet[5] = 0x09;  
  memcpy(&packet[6], argv[0], len);
 return !write_hci_command(HCI_PACKET_COMMAND, packet, 0xF4);
}
