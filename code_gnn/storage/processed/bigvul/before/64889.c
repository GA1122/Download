static int rle8_compress_row(struct rle_context *rlectx)
{
	size_t i;
	iw_byte dstbuf[2];
	iw_byte next_byte;
	int next_pix_is_trns;
	int num_trns = 0;  
	int retval = 0;

	rlectx->pending_data_start=0;
	rlectx->unc_len=0;
	rlectx->run_len=0;

	for(i=0;i<rlectx->img_width;i++) {

		next_byte = rlectx->srcrow[i];

		next_pix_is_trns = (rlectx->wctx->pal->entry[next_byte].a==0);

		if(num_trns>0 && !next_pix_is_trns) {
			rle_write_trns(rlectx,num_trns);
			num_trns=0;
		}
		else if(next_pix_is_trns) {
			if (rlectx->unc_len>0 || rlectx->run_len>0) {
				rle8_write_unc_and_run(rlectx);
			}
			num_trns++;
			continue;
		}


		if(rlectx->run_len>0 && next_byte==rlectx->run_byte) {
			rlectx->run_len++;
		}
		else if(rlectx->run_len==0) {
			rlectx->run_len = 1;
			rlectx->run_byte = next_byte;
		}
		else if(rlectx->unc_len==0 && rlectx->run_len==1) {
			rlectx->unc_len++;
			rlectx->run_byte = next_byte;
		}
		else if(rlectx->unc_len>0 && rlectx->run_len<(rlectx->unc_len==1 ? 3U : 4U)) {
			rlectx->unc_len += rlectx->run_len;
			rlectx->run_len = 0;
			if(rlectx->unc_len>=3 && (rlectx->unc_len&0x1)) {
				rlectx->unc_len++;
			}
			else {
				rlectx->run_len = 1;
				rlectx->run_byte = next_byte;
			}
		}
		else {
			rle8_write_unc_and_run(rlectx);
			rlectx->run_len = 1;
			rlectx->run_byte = next_byte;
		}


		if(rlectx->unc_len>=254) {
			rle8_write_unc(rlectx);
		}
		else if(rlectx->unc_len>0 && (rlectx->unc_len+rlectx->run_len)>254) {
			rle8_write_unc(rlectx);
		}
		else if(rlectx->run_len>=255) {
			rle8_write_unc_and_run(rlectx);
		}


		if((rlectx->unc_len&0x1) && rlectx->unc_len!=1) {
			iw_set_errorf(rlectx->ctx,"Internal: BMP RLE encode error 1");
			goto done;
		}

		if(rlectx->unc_len>252) {
			iw_set_error(rlectx->ctx,"Internal: BMP RLE encode error 2");
			goto done;
		}

		if(rlectx->run_len>254) {
			iw_set_error(rlectx->ctx,"Internal: BMP RLE encode error 3");
			goto done;
		}
	}

	rle8_write_unc_and_run(rlectx);

	dstbuf[0]=0x00;
	dstbuf[1]= (rlectx->cur_row==0)? 0x01 : 0x00;
	iwbmp_write(rlectx->wctx,dstbuf,2);
	rlectx->total_bytes_written+=2;

	retval = 1;

done:
	return retval;
}
