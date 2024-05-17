static void semicolon(js_State *J)
{
	if (J->lookahead == ';') {
		jsP_next(J);
		return;
	}
	if (J->newline || J->lookahead == '}' || J->lookahead == 0)
