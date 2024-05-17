GIFEncode(gdIOCtxPtr fp, int GWidth, int GHeight, int GInterlace, int Background, int Transparent, int BitsPerPixel, int *Red, int *Green, int *Blue, gdImagePtr im)
{
        int B;
        int RWidth, RHeight;
        int LeftOfs, TopOfs;
        int Resolution;
        int ColorMapSize;
        int InitCodeSize;
        int i;
		GifCtx ctx;

		memset(&ctx, 0, sizeof(ctx));
        ctx.Interlace = GInterlace;
		ctx.in_count = 1;

        ColorMapSize = 1 << BitsPerPixel;

        RWidth = ctx.Width = GWidth;
        RHeight = ctx.Height = GHeight;
        LeftOfs = TopOfs = 0;

        Resolution = BitsPerPixel;

         
        ctx.CountDown = (long)ctx.Width * (long)ctx.Height;

         
        ctx.Pass = 0;

         
        if( BitsPerPixel <= 1 )
                InitCodeSize = 2;
        else
                InitCodeSize = BitsPerPixel;

         
        ctx.curx = ctx.cury = 0;

         
        gdPutBuf(Transparent < 0 ? "GIF87a" : "GIF89a", 6, fp );

         
        gifPutWord( RWidth, fp );
        gifPutWord( RHeight, fp );

         
        B = 0x80;        

         
        B |= (Resolution - 1) << 5;

         
        B |= (BitsPerPixel - 1);

         
        gdPutC( B, fp );

         
        gdPutC( Background, fp );

         
        gdPutC( 0, fp );

         
        for( i=0; i<ColorMapSize; ++i ) {
                gdPutC( Red[i], fp );
                gdPutC( Green[i], fp );
                gdPutC( Blue[i], fp );
        }

	 
	if ( Transparent >= 0 ) {
	    gdPutC( '!', fp );
	    gdPutC( 0xf9, fp );
	    gdPutC( 4, fp );
	    gdPutC( 1, fp );
	    gdPutC( 0, fp );
	    gdPutC( 0, fp );
	    gdPutC( (unsigned char) Transparent, fp );
	    gdPutC( 0, fp );
	}

         
        gdPutC( ',', fp );

         

        gifPutWord( LeftOfs, fp );
        gifPutWord( TopOfs, fp );
        gifPutWord( ctx.Width, fp );
        gifPutWord( ctx.Height, fp );

         
        if( ctx.Interlace )
                gdPutC( 0x40, fp );
        else
                gdPutC( 0x00, fp );

         
        gdPutC( InitCodeSize, fp );

         
        compress( InitCodeSize+1, fp, im, &ctx );

         
        gdPutC( 0, fp );

         
        gdPutC( ';', fp );
}
