static struct fpm_child_s *fpm_child_alloc()  
{
	struct fpm_child_s *ret;

	ret = malloc(sizeof(struct fpm_child_s));

	if (!ret) {
		return 0;
	}

	memset(ret, 0, sizeof(*ret));
	ret->scoreboard_i = -1;
	return ret;
}
 