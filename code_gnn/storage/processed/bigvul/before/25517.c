static int __init nopfault(char *str)
{
	pfault_disable = 1;
	return 1;
}
