static struct page *new_node_page(struct page *page, unsigned long node, int **x)
{
	return alloc_pages_exact_node(node, GFP_HIGHUSER_MOVABLE, 0);
}
