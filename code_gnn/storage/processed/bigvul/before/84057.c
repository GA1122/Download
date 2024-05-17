static s32 SVC_ReadNal_header_extension(GF_BitStream *bs, SVC_NALUHeader *NalHeader)
{
	gf_bs_read_int(bs, 1);  
	NalHeader->idr_pic_flag = gf_bs_read_int(bs, 1);  
	NalHeader->priority_id = gf_bs_read_int(bs, 6);  
	gf_bs_read_int(bs, 1);  
	NalHeader->dependency_id = gf_bs_read_int(bs, 3);  
	NalHeader->quality_id = gf_bs_read_int(bs, 4);  
	NalHeader->temporal_id = gf_bs_read_int(bs, 3);  
	gf_bs_read_int(bs, 1);  
	gf_bs_read_int(bs, 1);  
	gf_bs_read_int(bs, 1);  
	gf_bs_read_int(bs, 2);  
	return 1;
}
