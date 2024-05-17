usage(char *cmd)
{
	int i;

	fprintf(stderr, "USAGE: %s", cmd);
	fprintf(stderr, " [OPTIONS] COMMAND [COMMAND ARGS]\n\n");

	fprintf(stderr,
			"OPTIONS:\n");
	fprintf(stderr, "  -i <VAL>\t\tinstance to connect to (0 - default)\n");

	fprintf(stderr,
			"COMMANDS:\n");
	for(i=0;i<commandListLen;i++){
		fprintf(stderr, "  %-21s %s\n",commandList[i].name,commandList[i].desc);
	}
	fflush(stderr);

}