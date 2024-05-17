int fpm_children_free(struct fpm_child_s *child)  
{
	struct fpm_child_s *next;

	for (; child; child = next) {
		next = child->next;
		fpm_child_close(child, 0  );
	}

	return 0;
}
 
