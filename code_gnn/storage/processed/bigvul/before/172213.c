static int help(int argc, char **argv) {
 if (!argc) {
    printf("No help command specified.\n");
 return 1;
 }

 const command_t *command = find_command(argv[0]);
 if (!command) {
    printf("No command named '%s'.\n", argv[0]);
 return 2;
 }

  printf("%s %s\n", argv[0], command->help);
 return 0;
}
