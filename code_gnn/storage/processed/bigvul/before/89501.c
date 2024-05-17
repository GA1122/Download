SWFShape_addSolidFillStyle(SWFShape shape, byte r, byte g, byte b, byte a)
{
	int  ret;

	SWFFillStyle fill = newSWFSolidFillStyle(r, g, b, a);
	
	ret = addFillStyle(shape, fill);
	if(ret < 0)  
	{
		destroySWFFillStyle(fill);
		return NULL;
	}
	else if(ret == shape->nFills)   
	{
		return fill;
	}
	else   
	{
		destroySWFFillStyle(fill);
		return shape->fills[ret];
	}
}
