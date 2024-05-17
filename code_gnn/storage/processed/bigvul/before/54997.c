static struct commit *get_revision_internal(struct rev_info *revs)
{
	struct commit *c = NULL;
	struct commit_list *l;

	if (revs->boundary == 2) {
		 
		c = pop_commit(&revs->commits);
		if (c)
			c->object.flags |= SHOWN;
		return c;
	}

	 
	if (revs->max_count) {
		c = get_revision_1(revs);
		if (c) {
			while (revs->skip_count > 0) {
				revs->skip_count--;
				c = get_revision_1(revs);
				if (!c)
					break;
			}
		}

		if (revs->max_count > 0)
			revs->max_count--;
	}

	if (c)
		c->object.flags |= SHOWN;

	if (!revs->boundary)
		return c;

	if (!c) {
		 
		revs->boundary = 2;

		 
		create_boundary_commit_list(revs);

		return get_revision_internal(revs);
	}

	 

	for (l = c->parents; l; l = l->next) {
		struct object *p;
		p = &(l->item->object);
		if (p->flags & (CHILD_SHOWN | SHOWN))
			continue;
		p->flags |= CHILD_SHOWN;
		gc_boundary(&revs->boundary_commits);
		add_object_array(p, NULL, &revs->boundary_commits);
	}

	return c;
}
