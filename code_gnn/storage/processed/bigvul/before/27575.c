static int __init setup_opl3(char *str)
{
         
	int ints[2];
	
	str = get_options(str, ARRAY_SIZE(ints), ints);
	
	io = ints[1];

	return 1;
}