struct xt_table *xt_find_table_lock(struct net *net, u_int8_t af,
				    const char *name)
{
	struct xt_table *t, *found = NULL;

	mutex_lock(&xt[af].mutex);
	list_for_each_entry(t, &net->xt.tables[af], list)
		if (strcmp(t->name, name) == 0 && try_module_get(t->me))
			return t;

	if (net == &init_net)
		goto out;

	 
	list_for_each_entry(t, &init_net.xt.tables[af], list) {
		if (strcmp(t->name, name))
			continue;
		if (!try_module_get(t->me))
			return NULL;

		mutex_unlock(&xt[af].mutex);
		if (t->table_init(net) != 0) {
			module_put(t->me);
			return NULL;
		}

		found = t;

		mutex_lock(&xt[af].mutex);
		break;
	}

	if (!found)
		goto out;

	 
	list_for_each_entry(t, &net->xt.tables[af], list)
		if (strcmp(t->name, name) == 0)
			return t;

	module_put(found->me);
 out:
	mutex_unlock(&xt[af].mutex);
	return NULL;
}