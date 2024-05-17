static int iwbmp_read_palette(struct iwbmprcontext *rctx)
{
	size_t i;
	iw_byte buf[4*256];
	size_t b;
	unsigned int valid_palette_entries;
	size_t valid_palette_nbytes;

	b = (rctx->bmpversion==2) ? 3 : 4;  

	if(rctx->infoheader_size==64) {
		if(rctx->fileheader_size + rctx->infoheader_size + rctx->palette_entries*3 ==
			rctx->bfOffBits)
		{
			iw_warning(rctx->ctx,"BMP bitmap overlaps colormap; assuming colormap uses 3 bytes per entry instead of 4");
			b = 3;
			rctx->palette_nbytes = 3*rctx->palette_entries;
		}
	}

	valid_palette_entries = (rctx->palette_entries<=256) ? rctx->palette_entries : 256;
	valid_palette_nbytes = valid_palette_entries * b;


	if(!iwbmp_read(rctx,buf,valid_palette_nbytes)) return 0;
	rctx->palette.num_entries = valid_palette_entries;
	for(i=0;i<valid_palette_entries;i++) {
		rctx->palette.entry[i].b = buf[i*b+0];
		rctx->palette.entry[i].g = buf[i*b+1];
		rctx->palette.entry[i].r = buf[i*b+2];
		rctx->palette.entry[i].a = 255;
	}

	if(rctx->palette_nbytes > valid_palette_nbytes) {
		iwbmp_skip_bytes(rctx, rctx->palette_nbytes - valid_palette_nbytes);
	}
	return 1;
}