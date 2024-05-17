static int opensc_set_conf_entry(const char *config)
{
	scconf_block *conf_block = NULL, **blocks;
	char *buffer = NULL;
	char *section = NULL;
	char *name = NULL;
	char *key = NULL;
	char *value = NULL;
	int r = 0;

	if (ctx->conf == NULL) {
		r = ENOENT;
		goto cleanup;
	}

	if ((buffer = strdup(config)) == NULL) {
		r = ENOMEM;
		goto cleanup;
	}

	section = buffer;
	name = strchr(section+1, ':');
	key = name == NULL ? NULL : strchr(name+1, ':');
	value = key == NULL ? NULL : strchr(key+1, ':');
	if (value == NULL) {
		r = EINVAL;
		goto cleanup;
	}
	*name = '\0';
	name++;
	*key = '\0';
	key++;
	*value = '\0';
	value++;

	blocks = scconf_find_blocks(ctx->conf, NULL, section, name);
	if (blocks && blocks[0])
		conf_block = blocks[0];
	free(blocks);
	if (conf_block != NULL) {
		scconf_item *item;

		for (item = conf_block->items; item != NULL; item = item->next) {
			scconf_list *list;

			if ((item->type != SCCONF_ITEM_TYPE_VALUE)
			   || (strcmp(item->key, key) != 0))
				continue;
			list = item->value.list;
			scconf_list_destroy(list);
			list = NULL;
			scconf_list_add(&list, value);
			item->value.list = list;
			break;
		}
		if (item == NULL)
			scconf_put_str(conf_block, key, value);
	}

	 
	if ((r = scconf_write(ctx->conf, ctx->conf->filename)) != 0) {
		fprintf(stderr, "scconf_write(): %s\n", strerror(r));
		goto cleanup;
	}

	r = 0;

cleanup:

	if (buffer != NULL)
		free(buffer);

	return r;
}