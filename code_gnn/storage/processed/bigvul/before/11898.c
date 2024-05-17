static js_Ast *parameters(js_State *J)
{
	js_Ast *head, *tail;
	if (J->lookahead == ')')
		return NULL;
	head = tail = LIST(identifier(J));
	while (jsP_accept(J, ',')) {
		tail = tail->b = LIST(identifier(J));
	}
	return jsP_list(head);
}
