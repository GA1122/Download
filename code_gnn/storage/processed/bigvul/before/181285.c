 static int get_exif_tag_dbl_value(struct iw_exif_state *e, unsigned int tag_pos,
 	double *pv)
 {
 	unsigned int field_type;
 	unsigned int value_count;
  	unsigned int value_pos;
  	unsigned int numer, denom;
  
	field_type = iw_get_ui16_e(&e->d[tag_pos+2],e->endian);
	value_count = iw_get_ui32_e(&e->d[tag_pos+4],e->endian);
// 	field_type = get_exif_ui16(e, tag_pos+2);
// 	value_count = get_exif_ui32(e, tag_pos+4);
  
  	if(value_count!=1) return 0;
  
 	if(field_type!=5) return 0;  
 
  
	value_pos = iw_get_ui32_e(&e->d[tag_pos+8],e->endian);
// 	value_pos = get_exif_ui32(e, tag_pos+8);
  	if(value_pos > e->d_len-8) return 0;
  
	numer = iw_get_ui32_e(&e->d[value_pos  ],e->endian);
	denom = iw_get_ui32_e(&e->d[value_pos+4],e->endian);
// 	numer = get_exif_ui32(e, value_pos);
// 	denom = get_exif_ui32(e, value_pos+4);
  	if(denom==0) return 0;
  
  	*pv = ((double)numer)/denom;
 	return 1;
 }