static void jsP_next(js_State *J)
{
	J->lookahead = jsY_lex(J);
	J->astline = J->lexline;
}
