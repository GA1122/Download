static void update_write_suppress_output(wStream* s, BYTE allow,
        const RECTANGLE_16* area)
{
	Stream_Write_UINT8(s, allow);  
	 
	Stream_Zero(s, 3);  

	if (allow > 0)
	{
		Stream_Write_UINT16(s, area->left);  
		Stream_Write_UINT16(s, area->top);  
		Stream_Write_UINT16(s, area->right);  
		Stream_Write_UINT16(s, area->bottom);  
	}
}
