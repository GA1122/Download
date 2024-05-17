static void give_pages(struct receive_queue *rq, struct page *page)
{
	struct page *end;

	 
	for (end = page; end->private; end = (struct page *)end->private);
	end->private = (unsigned long)rq->pages;
	rq->pages = page;
}
