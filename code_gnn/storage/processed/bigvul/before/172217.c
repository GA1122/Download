static int set_pcm_loopback(int argc, char **argv) {
 if (argc != 1) {
    printf("PCM loopback mode not specified.\n");
 return 1;
 }

 if (strcmp(argv[0], "true") && strcmp(argv[0], "false")) {
    printf("Invalid PCM mode '%s'.\n", argv[0]);
 return 2;
 }

 uint8_t packet[] = { 0x24, 0xFC, 0x01, 0x00 };
 if (argv[0][0] == 't')
    packet[ARRAY_SIZE(packet) - 1] = 0x01;

 return !write_hci_command(HCI_PACKET_COMMAND, packet, ARRAY_SIZE(packet));
}
