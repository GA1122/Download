int main(int argc, char *argv[])
{
	int ret = -1;
	 
	int nargs = 5, cnt = 0;
	char *newargv[6];

#ifdef FORTRAVIS
	 
	if (glib_check_version (2, 36, 0) != NULL)
		g_type_init ();
#endif

	 
	swallow_arg(&argc, argv, "-s");
	swallow_arg(&argc, argv, "-f");
	swallow_option(&argc, argv, "-o", "allow_other");

	if (argc == 2  && strcmp(argv[1], "--version") == 0) {
		fprintf(stderr, "%s\n", VERSION);
		exit(0);
	}
	if (argc != 2 || is_help(argv[1]))
		usage(argv[0]);

	newargv[cnt++] = argv[0];
	newargv[cnt++] = "-f";
	newargv[cnt++] = "-o";
	newargv[cnt++] = "allow_other,direct_io,entry_timeout=0.5,attr_timeout=0.5";
	newargv[cnt++] = argv[1];
	newargv[cnt++] = NULL;

	if (!cgfs_setup_controllers())
		goto out;

	ret = fuse_main(nargs, newargv, &lxcfs_ops, NULL);

out:
	return ret;
}
