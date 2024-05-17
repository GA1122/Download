static js_Ast *jsP_newnode(js_State *J, enum js_AstType type, js_Ast *a, js_Ast *b, js_Ast *c, js_Ast *d)
{
	js_Ast *node = js_malloc(J, sizeof *node);

	node->type = type;
	node->line = J->astline;
	node->a = a;
	node->b = b;
	node->c = c;
	node->d = d;
	node->number = 0;
	node->string = NULL;
	node->jumps = NULL;
	node->casejump = 0;

	node->parent = NULL;
	if (a) a->parent = node;
	if (b) b->parent = node;
	if (c) c->parent = node;
	if (d) d->parent = node;

	node->gcnext = J->gcast;
	J->gcast = node;

	return node;
}
