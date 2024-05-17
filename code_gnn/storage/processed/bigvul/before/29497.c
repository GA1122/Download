static int exitcode_proc_show(struct seq_file *m, void *v)
{
	int val;

	 
	val = uml_exitcode;
	seq_printf(m, "%d\n", val);
	return 0;
}
