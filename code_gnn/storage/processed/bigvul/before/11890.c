static js_Ast *jsP_list(js_Ast *head)
{
	 
	js_Ast *prev = head, *node = head->b;
	while (node) {
		node->parent = prev;
		prev = node;
		node = node->b;
	}
	return head;
}
