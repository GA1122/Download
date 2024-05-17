destroy_restrict_node(
	restrict_node *my_node
	)
{
	 
	destroy_address_node(my_node->addr);
	destroy_address_node(my_node->mask);
	destroy_int_fifo(my_node->flags);
	free(my_node);
}
