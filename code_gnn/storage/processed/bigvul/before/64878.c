static void iwbmp_write_palette(struct iwbmpwcontext *wctx)
{
	int i,k;
	iw_byte buf[4];

	if(wctx->palentries<1) return;

	buf[3] = 0;  

	for(i=0;i<wctx->palentries;i++) {
		if(i<wctx->pal->num_entries) {
			if(wctx->pal->entry[i].a == 0) {
				if(wctx->img->has_bkgdlabel) {
					for(k=0;k<3;k++) {
						buf[k] = (iw_byte)iw_color_get_int_sample(&wctx->img->bkgdlabel,2-k,255);
					}
				}
				else {
					buf[0] = 255;
					buf[1] = 0;
					buf[2] = 255;
				}
			}
			else {
				buf[0] = wctx->pal->entry[i].b;
				buf[1] = wctx->pal->entry[i].g;
				buf[2] = wctx->pal->entry[i].r;
			}
		}
		else {
			buf[0] = buf[1] = buf[2] = 0;
		}
		if(wctx->bmpversion==2)
			iwbmp_write(wctx,buf,3);  
		else
			iwbmp_write(wctx,buf,4);
	}
}