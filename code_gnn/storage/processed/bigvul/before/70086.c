static OPJ_BOOL bmp_read_rle4_data(FILE* IN, OPJ_UINT8* pData, OPJ_UINT32 stride, OPJ_UINT32 width, OPJ_UINT32 height)
{
	OPJ_UINT32 x, y;
	OPJ_UINT8 *pix;
	const OPJ_UINT8 *beyond;
	
	beyond = pData + stride * height;
	pix = pData;
	x = y = 0U;
	while(y < height)
	{
		int c = getc(IN);
		if(c == EOF) break;
		
		if(c) { 
			int j;
			OPJ_UINT8 c1 = (OPJ_UINT8)getc(IN);
		
			for (j = 0; (j < c) && (x < width) && ((OPJ_SIZE_T)pix < (OPJ_SIZE_T)beyond); j++, x++, pix++) {
				*pix = (OPJ_UINT8)((j&1) ? (c1 & 0x0fU) : ((c1>>4)&0x0fU));
			}
		}
		else {  
			c = getc(IN);
			if(c == EOF) break;
		
			if(c == 0x00) {  
				x = 0;  y++;  pix = pData + y * stride;
			}
			else if(c == 0x01) {  
				break;
			}
			else if(c == 0x02) {  
				c = getc(IN);  x += (OPJ_UINT32)c;
				c = getc(IN);  y += (OPJ_UINT32)c;
				pix = pData + y * stride + x;
			}
			else {  
				int j;
				OPJ_UINT8 c1 = 0U;
				
				for (j = 0; (j < c) && (x < width) && ((OPJ_SIZE_T)pix < (OPJ_SIZE_T)beyond); j++, x++, pix++) {
					if((j&1) == 0) {
							c1 = (OPJ_UINT8)getc(IN);
					}
					*pix =  (OPJ_UINT8)((j&1) ? (c1 & 0x0fU) : ((c1>>4)&0x0fU));
				}
				if(((c&3) == 1) || ((c&3) == 2)) {  
						getc(IN);
				}
			}
		}
	}   
	return OPJ_TRUE;
}
