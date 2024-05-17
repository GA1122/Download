  static int get_exif_tag_int_value(struct iw_exif_state *e, unsigned int tag_pos,
 	unsigned int *pv)
 {
  	unsigned int field_type;
  	unsigned int value_count;
  
	field_type = iw_get_ui16_e(&e->d[tag_pos+2],e->endian);
	value_count = iw_get_ui32_e(&e->d[tag_pos+4],e->endian);
// 	field_type = get_exif_ui16(e, tag_pos+2);
// 	value_count = get_exif_ui32(e, tag_pos+4);
  
  	if(value_count!=1) return 0;
  
  	if(field_type==3) {  
		*pv = iw_get_ui16_e(&e->d[tag_pos+8],e->endian);
// 		*pv = get_exif_ui16(e, tag_pos+8);
  		return 1;
  	}
  	else if(field_type==4) {  
		*pv = iw_get_ui32_e(&e->d[tag_pos+8],e->endian);
// 		*pv = get_exif_ui32(e, tag_pos+8);
  		return 1;
  	}
  
 	return 0;
 }