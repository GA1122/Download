R_API void r_flag_item_free(RFlagItem *item) {
	if (item) {
		free (item->color);
		free (item->comment);
		free (item->alias);
		 
		if (item->name != item->realname) {
			free (item->name);
		}
		free (item->realname);
		free (item);
	}
}
