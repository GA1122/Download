static void fpm_child_unlink(struct fpm_child_s *child)  
{
	--child->wp->running_children;
	--fpm_globals.running_children;

	if (child->prev) {
		child->prev->next = child->next;
	} else {
		child->wp->children = child->next;
	}

	if (child->next) {
		child->next->prev = child->prev;
	}
}
 