 dissect_spoolss_keybuffer(tvbuff_t *tvb, int offset, packet_info *pinfo,
 			  proto_tree *tree, dcerpc_info *di, guint8 *drep)
 {
 	guint32 size;
 	int end_offset;
 
 	if (di->conformant_run)
 		return offset;
 
 	 
 
 	offset = dissect_ndr_uint32(tvb, offset, pinfo, tree, di, drep,
 				    hf_keybuffer_size, &size);
 
 	end_offset = offset + (size*2);
 	if (end_offset < offset) {
 		 
  		end_offset = tvb_reported_length_remaining(tvb, offset) + 1;
  	}
  
	while (offset < end_offset)
// 	while (offset > 0 && offset < end_offset) {
  		offset = dissect_spoolss_uint16uni(
  			tvb, offset, pinfo, tree, drep, NULL, hf_keybuffer);
// 	}
  
  	return offset;
  }