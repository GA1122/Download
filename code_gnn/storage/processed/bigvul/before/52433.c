struct xt_table *xt_register_table(struct net *net,
				   const struct xt_table *input_table,
				   struct xt_table_info *bootstrap,
				   struct xt_table_info *newinfo)
{
	int ret;
	struct xt_table_info *private;
	struct xt_table *t, *table;

	 
	table = kmemdup(input_table, sizeof(struct xt_table), GFP_KERNEL);
	if (!table) {
		ret = -ENOMEM;
		goto out;
	}

	mutex_lock(&xt[table->af].mutex);
	 
	list_for_each_entry(t, &net->xt.tables[table->af], list) {
		if (strcmp(t->name, table->name) == 0) {
			ret = -EEXIST;
			goto unlock;
		}
	}

	 
	table->private = bootstrap;

	if (!xt_replace_table(table, 0, newinfo, &ret))
		goto unlock;

	private = table->private;
	pr_debug("table->private->number = %u\n", private->number);

	 
	private->initial_entries = private->number;

	list_add(&table->list, &net->xt.tables[table->af]);
	mutex_unlock(&xt[table->af].mutex);
	return table;

unlock:
	mutex_unlock(&xt[table->af].mutex);
	kfree(table);
out:
	return ERR_PTR(ret);
}