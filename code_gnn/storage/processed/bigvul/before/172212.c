static const command_t *find_command(const char *name) {
 for (size_t i = 0; i < ARRAY_SIZE(commands); ++i)
 if (!strcmp(commands[i].name, name))
 return &commands[i];
 return NULL;
}
