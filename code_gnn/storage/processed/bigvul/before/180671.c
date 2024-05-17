 NeXTDecode(TIFF* tif, uint8* buf, tmsize_t occ, uint16 s)
 {
 	static const char module[] = "NeXTDecode";
 	unsigned char *bp, *op;
 	tmsize_t cc;
 	uint8* row;
 	tmsize_t scanline, n;
 
 	(void) s;
 	 
 	for (op = (unsigned char*) buf, cc = occ; cc-- > 0;)
 		*op++ = 0xff;
 
 	bp = (unsigned char *)tif->tif_rawcp;
 	cc = tif->tif_rawcc;
 	scanline = tif->tif_scanlinesize;
 	if (occ % scanline)
 	{
 		TIFFErrorExt(tif->tif_clientdata, module, "Fractional scanlines cannot be read");
 		return (0);
 	}
 	for (row = buf; cc > 0 && occ > 0; occ -= scanline, row += scanline) {
 		n = *bp++, cc--;
 		switch (n) {
 		case LITERALROW:
 			 
 			if (cc < scanline)
 				goto bad;
 			_TIFFmemcpy(row, bp, scanline);
 			bp += scanline;
 			cc -= scanline;
 			break;
 		case LITERALSPAN: {
 			tmsize_t off;
 			 
 			if( cc < 4 )
 				goto bad;
 			off = (bp[0] * 256) + bp[1];
 			n = (bp[2] * 256) + bp[3];
 			if (cc < 4+n || off+n > scanline)
 				goto bad;
 			_TIFFmemcpy(row+off, bp+4, n);
 			bp += 4+n;
 			cc -= 4+n;
 			break;
 		}
 		default: {
 			uint32 npixels = 0, grey;
  			uint32 imagewidth = tif->tif_dir.td_imagewidth;
              if( isTiled(tif) )
                  imagewidth = tif->tif_dir.td_tilewidth;
//             tmsize_t op_offset = 0;
  
  			 
 			op = row;
 			for (;;) {
 				grey = (uint32)((n>>6) & 0x3);
 				n &= 0x3f;
 				 
				while (n-- > 0 && npixels < imagewidth)
// 				while (n-- > 0 && npixels < imagewidth && op_offset < scanline)
  					SETPIXEL(op, grey);
  				if (npixels >= imagewidth)
  					break;
//                 if (op_offset >= scanline ) {
//                     TIFFErrorExt(tif->tif_clientdata, module, "Invalid data for scanline %ld",
//                         (long) tif->tif_row);
//                     return (0);
//                 }
  				if (cc == 0)
  					goto bad;
  				n = *bp++, cc--;
 			}
 			break;
 		}
 		}
 	}
 	tif->tif_rawcp = (uint8*) bp;
 	tif->tif_rawcc = cc;
 	return (1);
 bad:
 	TIFFErrorExt(tif->tif_clientdata, module, "Not enough data for scanline %ld",
 	    (long) tif->tif_row);
 	return (0);
 }