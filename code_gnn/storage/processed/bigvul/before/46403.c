static int allocate_budget(struct ubifs_info *c, struct page *page,
			   struct ubifs_inode *ui, int appending)
{
	struct ubifs_budget_req req = { .fast = 1 };

	if (PagePrivate(page)) {
		if (!appending)
			 
			return 0;

		mutex_lock(&ui->ui_mutex);
		if (ui->dirty)
			 
			return 0;

		 
		req.dirtied_ino = 1;
	} else {
		if (PageChecked(page))
			 
			req.new_page = 1;
		else
			 
			req.dirtied_page = 1;

		if (appending) {
			mutex_lock(&ui->ui_mutex);
			if (!ui->dirty)
				 
				req.dirtied_ino = 1;
		}
	}

	return ubifs_budget_space(c, &req);
}
