static int rle4_compress_row(struct rle_context *rlectx)
{
	size_t i;
	iw_byte dstbuf[2];
	iw_byte next_pix;
	int next_pix_is_trns;
	int num_trns = 0;  
	int retval = 0;
	iw_byte tmpb;

	rlectx->pending_data_start=0;
	rlectx->unc_len=0;
	rlectx->run_len=0;

	for(i=0;i<rlectx->img_width;i++) {

		next_pix = rlectx->srcrow[i];

		next_pix_is_trns = (rlectx->wctx->pal->entry[next_pix].a==0);
		if(num_trns>0 && !next_pix_is_trns) {
			rle_write_trns(rlectx,num_trns);
			num_trns=0;
		}
		else if(next_pix_is_trns) {
			if (rlectx->unc_len>0 || rlectx->run_len>0) {
				rle4_write_unc_and_run(rlectx);
			}
			num_trns++;
			continue;
		}


		if(rlectx->run_len==0) {
			rlectx->run_len = 1;
			rlectx->run_byte = next_pix<<4;
		}
		else if(rlectx->run_len==1) {
			rlectx->run_byte |= next_pix;
			rlectx->run_len++;
		}
		else if(rlectx->run_len>=2 && (rlectx->run_len&1)==0 && next_pix==(rlectx->run_byte>>4)) {
			rlectx->run_len++;
		}
		else if(rlectx->run_len>=3 && (rlectx->run_len&1) && next_pix==(rlectx->run_byte&0x0f)) {
			rlectx->run_len++;
		}
		else if(rlectx->unc_len==0 && rlectx->run_len==2) {
			rlectx->unc_len+=rlectx->run_len;
			rlectx->run_byte = next_pix<<4;
			rlectx->run_len = 1;
		}
		else if(ok_to_move_to_unc(rlectx)) {
			rlectx->unc_len += rlectx->run_len-1;
 
			if((rlectx->run_len&1)==0)
				rlectx->run_byte = (rlectx->run_byte&0x0f)<<4;
			else
				rlectx->run_byte = (rlectx->run_byte&0xf0);

			rlectx->run_len = 2;
			rlectx->run_byte |= next_pix;
		}
		else {
			rle4_write_unc_and_run(rlectx);
			rlectx->run_len = 1;
			rlectx->run_byte = next_pix<<4;
		}

		while(rlectx->unc_len>0 && rlectx->run_len>0 && rle4_get_incr_unc_cost(rlectx)==0) {
			rlectx->unc_len++;
			rlectx->run_len--;
			tmpb = rlectx->run_byte;
			rlectx->run_byte = (tmpb>>4) | ((tmpb&0x0f)<<4);
			if(rlectx->run_len==1) rlectx->run_byte &= 0xf0;
		}


		if(rlectx->run_len>=255) {
			rle4_write_unc_and_run(rlectx);
		}


		if(rlectx->run_len>255) {
			iw_set_error(rlectx->ctx,"Internal: BMP RLE encode error 3");
			goto done;
		}
	}

	rle4_write_unc_and_run(rlectx);

	dstbuf[0]=0x00;
	dstbuf[1]= (rlectx->cur_row==0)? 0x01 : 0x00;
	iwbmp_write(rlectx->wctx,dstbuf,2);
	rlectx->total_bytes_written+=2;

	retval = 1;

done:
	return retval;
}
