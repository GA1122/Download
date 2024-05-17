static void update_write_refresh_rect(wStream* s, BYTE count,
                                      const RECTANGLE_16* areas)
{
	int i;
	Stream_Write_UINT8(s, count);  
	Stream_Seek(s, 3);  

	for (i = 0; i < count; i++)
	{
		Stream_Write_UINT16(s, areas[i].left);  
		Stream_Write_UINT16(s, areas[i].top);  
		Stream_Write_UINT16(s, areas[i].right);  
		Stream_Write_UINT16(s, areas[i].bottom);  
	}
}
