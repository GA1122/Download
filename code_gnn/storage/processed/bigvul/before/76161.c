get_cwd(void)
{
	char *dir = MALLOC(PATH_MAX);

	 
	return getcwd(dir, PATH_MAX);
}
