static js_Ast *arguments(js_State *J)
{
	js_Ast *head, *tail;
	if (J->lookahead == ')')
		return NULL;
	head = tail = LIST(assignment(J, 0));
	while (jsP_accept(J, ',')) {
		tail = tail->b = LIST(assignment(J, 0));
	}
	return jsP_list(head);
}
