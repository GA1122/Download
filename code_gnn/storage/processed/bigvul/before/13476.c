void js_rot2(js_State *J)
{
	 
	js_Value tmp = STACK[TOP-1];	 
	STACK[TOP-1] = STACK[TOP-2];	 
	STACK[TOP-2] = tmp;		 
}
