vips_foreign_load_gif_render_line( VipsForeignLoadGif *gif,
	int width, VipsPel * restrict q, VipsPel * restrict p )
{
	guint32 *iq;
	int x;

	iq = (guint32 *) q;
	for( x = 0; x < width; x++ ) {
		VipsPel v = p[x];
		
		if( v == gif->transparency ) {
			 
			if( gif->dispose != DISPOSE_DO_NOT ) 
				iq[x] = 0;
		}
		else
			 
			iq[x] = gif->cmap[v];
	}
}
