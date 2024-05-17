static int label_localboot(struct pxe_label *label)
{
	char *localcmd;

	localcmd = from_env("localcmd");

	if (!localcmd)
		return -ENOENT;

	if (label->append) {
		char bootargs[CONFIG_SYS_CBSIZE];

		cli_simple_process_macros(label->append, bootargs);
		env_set("bootargs", bootargs);
	}

	debug("running: %s\n", localcmd);

	return run_command_list(localcmd, strlen(localcmd), 0);
}
