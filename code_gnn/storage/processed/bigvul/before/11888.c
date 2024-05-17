static void jsP_freejumps(js_State *J, js_JumpList *node)
{
	while (node) {
		js_JumpList *next = node->next;
		js_free(J, node);
		node = next;
	}
}
