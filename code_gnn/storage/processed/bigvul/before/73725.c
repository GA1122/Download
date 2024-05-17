static int createFromTiffLines(TIFF *tif, gdImagePtr im, uint16 bps, uint16 photometric,
                               char has_alpha, char is_bw, int extra)
{
	uint16  planar;
	uint32 im_height, im_width, y;

	unsigned char *buffer;

	if (!TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &planar)) {
		planar = PLANARCONFIG_CONTIG;
	}

	if (!TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &im_height)) {
		gd_error("Can't fetch TIFF height\n");
		return FALSE;
	}

	if (!TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &im_width)) {
		gd_error("Can't fetch TIFF width \n");
		return FALSE;
	}

	buffer = (unsigned char *)gdMalloc(im_width * 4);
	if (!buffer) {
		return GD_FAILURE;
	}
	if (planar == PLANARCONFIG_CONTIG) {
		switch (bps) {
		case 16:
			 
			break;

		case 8:
			for (y = 0; y < im_height; y++ ) {
				if (!TIFFReadScanline (tif, buffer, y, 0)) {
					gd_error("Error while reading scanline %i", y);
					break;
				}
				 
				readTiff8bit(buffer, im, photometric, 0, y, im_width, 1, has_alpha, extra, 0);
			}
			break;

		default:
			if (is_bw) {
				for (y = 0; y < im_height; y++ ) {
					if (!TIFFReadScanline (tif, buffer, y, 0)) {
						gd_error("Error while reading scanline %i", y);
						break;
					}
					 
					readTiffBw(buffer, im, photometric, 0, y, im_width, 1, has_alpha, extra, 0);
				}
			} else {
				 
			}
			break;
		}
	} else {
		 
	}

	gdFree(buffer);
	return GD_SUCCESS;
}
