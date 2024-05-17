sort_dir_list(struct fixup_entry *p)
{
	struct fixup_entry *a, *b, *t;

	if (p == NULL)
		return (NULL);
	 
	if (p->next == NULL)
		return (p);

	 
	t = p;
	a = p->next->next;
	while (a != NULL) {
		 
		a = a->next;
		if (a != NULL)
			a = a->next;
		t = t->next;
	}
	 
	b = t->next;
	t->next = NULL;
	a = p;

	 
	a = sort_dir_list(a);
	b = sort_dir_list(b);

	 
	 
	if (strcmp(a->name, b->name) > 0) {
		t = p = a;
		a = a->next;
	} else {
		t = p = b;
		b = b->next;
	}

	 
	while (a != NULL && b != NULL) {
		if (strcmp(a->name, b->name) > 0) {
			t->next = a;
			a = a->next;
		} else {
			t->next = b;
			b = b->next;
		}
		t = t->next;
	}

	 
	if (a != NULL)
		t->next = a;
	if (b != NULL)
		t->next = b;

	return (p);
}
