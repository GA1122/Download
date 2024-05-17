int js_pcall(js_State *J, int n)
{
	int savetop = TOP - n - 2;
	if (js_try(J)) {
		 
		STACK[savetop] = STACK[TOP-1];
		TOP = savetop + 1;
		return 1;
	}
	js_call(J, n);
	js_endtry(J);
	return 0;
}
