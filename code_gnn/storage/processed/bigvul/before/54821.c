static void add_descendants_to_write_order(struct object_entry **wo,
					   unsigned int *endp,
					   struct object_entry *e)
{
	int add_to_order = 1;
	while (e) {
		if (add_to_order) {
			struct object_entry *s;
			 
			add_to_write_order(wo, endp, e);
			 
			for (s = e->delta_sibling; s; s = s->delta_sibling) {
				add_to_write_order(wo, endp, s);
			}
		}
		 
		if (e->delta_child) {
			add_to_order = 1;
			e = e->delta_child;
		} else {
			add_to_order = 0;
			 
			if (e->delta_sibling) {
				e = e->delta_sibling;
				continue;
			}
			 
			e = e->delta;
			while (e && !e->delta_sibling) {
				 
				e = e->delta;
			}
			if (!e) {
				 
				return;
			}
			 
			e = e->delta_sibling;
		}
	};
}
