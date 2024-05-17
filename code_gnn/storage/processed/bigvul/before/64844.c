static int bmpr_read_rle_internal(struct iwbmprcontext *rctx)
{
	int retval = 0;
	int pos_x, pos_y;
	iw_byte buf[255];
	size_t n_pix;
	size_t n_bytes;
	size_t i;
	size_t pal_index;

	pos_x = 0;
	pos_y = 0;

	iw_zeromem(rctx->img->pixels,rctx->img->bpr*rctx->img->height);

	while(1) {
		if(pos_y>rctx->img->height-1) break;
		if(pos_y==rctx->img->height-1 && pos_x>=rctx->img->width) break;

		if(!iwbmp_read(rctx,buf,2)) goto done;
		if(buf[0]==0) {
			if(buf[1]==0) {
				pos_y++;
				pos_x=0;
			}
			else if(buf[1]==1) {
				break;
			}
			else if(buf[1]==2) {
				if(!iwbmp_read(rctx,buf,2)) goto done;

				if(pos_x<rctx->img->width) pos_x += buf[0];
				pos_y += buf[1];
			}
			else {
				n_pix = (size_t)buf[1];  
				if(rctx->compression==IWBMP_BI_RLE4) {
					n_bytes = ((n_pix+3)/4)*2;
				}
				else {
					n_bytes = ((n_pix+1)/2)*2;
				}
				if(!iwbmp_read(rctx,buf,n_bytes)) goto done;
				for(i=0;i<n_pix;i++) {
					if(pos_x<rctx->img->width) {
						if(rctx->compression==IWBMP_BI_RLE4) {
							pal_index = (i%2) ? buf[i/2]&0x0f : buf[i/2]>>4;
						}
						else {
							pal_index = buf[i];
						}
						rctx->img->pixels[rctx->img->bpr*pos_y + pos_x*4 + 0] = rctx->palette.entry[pal_index].r;
						rctx->img->pixels[rctx->img->bpr*pos_y + pos_x*4 + 1] = rctx->palette.entry[pal_index].g;
						rctx->img->pixels[rctx->img->bpr*pos_y + pos_x*4 + 2] = rctx->palette.entry[pal_index].b;
						rctx->img->pixels[rctx->img->bpr*pos_y + pos_x*4 + 3] = 255;
						pos_x++;
					}
				}
			}
		}
		else {
			n_pix = (size_t)buf[0];
			for(i=0;i<n_pix;i++) {
				if(pos_x<rctx->img->width) {
					if(rctx->compression==IWBMP_BI_RLE4) {
						pal_index = (i%2) ? buf[1]&0x0f : buf[1]>>4;
					}
					else {
						pal_index = buf[1];
					}
					rctx->img->pixels[rctx->img->bpr*pos_y + pos_x*4 + 0] = rctx->palette.entry[pal_index].r;
					rctx->img->pixels[rctx->img->bpr*pos_y + pos_x*4 + 1] = rctx->palette.entry[pal_index].g;
					rctx->img->pixels[rctx->img->bpr*pos_y + pos_x*4 + 2] = rctx->palette.entry[pal_index].b;
					rctx->img->pixels[rctx->img->bpr*pos_y + pos_x*4 + 3] = 255;
					pos_x++;
				}
			}
		}
	}

	retval = 1;
done:
	return retval;
}
