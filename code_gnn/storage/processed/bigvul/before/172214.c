int main(int argc, char **argv) {
 if (argc < 2) {
    usage(argv[0]);
 return -1;
 }

 const command_t *command = find_command(argv[1]);
 if (!command) {
    printf("Unrecognized command '%s'.\n", argv[1]);
 return -2;
 }

 if (!command->handler) {
    printf("Unhandled command '%s'.\n", argv[1]);
 return -3;
 }

 return command->handler(argc - 2, &argv[2]);
}
