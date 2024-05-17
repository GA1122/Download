BOOL rectangle_is_empty(const RECTANGLE_16* rect)
{
	 
	return ((rect->left == rect->right) || (rect->top == rect->bottom)) ? TRUE : FALSE;
}
