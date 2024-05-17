TIFFWriteScanline(TIFF* tif, void* buf, uint32 row, uint16 sample)
{
	static const char module[] = "TIFFWriteScanline";
	register TIFFDirectory *td;
	int status, imagegrew = 0;
	uint32 strip;

	if (!WRITECHECKSTRIPS(tif, module))
		return (-1);
	 
	if (!BUFFERCHECK(tif))
		return (-1);
        tif->tif_flags |= TIFF_BUF4WRITE;  

	td = &tif->tif_dir;
	 
	if (row >= td->td_imagelength) {	 
		if (td->td_planarconfig == PLANARCONFIG_SEPARATE) {
			TIFFErrorExt(tif->tif_clientdata, module,
			    "Can not change \"ImageLength\" when using separate planes");
			return (-1);
		}
		td->td_imagelength = row+1;
		imagegrew = 1;
	}
	 
	if (td->td_planarconfig == PLANARCONFIG_SEPARATE) {
		if (sample >= td->td_samplesperpixel) {
			TIFFErrorExt(tif->tif_clientdata, module,
			    "%lu: Sample out of range, max %lu",
			    (unsigned long) sample, (unsigned long) td->td_samplesperpixel);
			return (-1);
		}
		strip = sample*td->td_stripsperimage + row/td->td_rowsperstrip;
	} else
		strip = row / td->td_rowsperstrip;
	 
	if (strip >= td->td_nstrips && !TIFFGrowStrips(tif, 1, module))
		return (-1);
	if (strip != tif->tif_curstrip) {
		 
		if (!TIFFFlushData(tif))
			return (-1);
		tif->tif_curstrip = strip;
		 
		if (strip >= td->td_stripsperimage && imagegrew)
			td->td_stripsperimage =
			    TIFFhowmany_32(td->td_imagelength,td->td_rowsperstrip);
                if (td->td_stripsperimage == 0) {
                        TIFFErrorExt(tif->tif_clientdata, module, "Zero strips per image");
                        return (-1);
                }
		tif->tif_row =
		    (strip % td->td_stripsperimage) * td->td_rowsperstrip;
		if ((tif->tif_flags & TIFF_CODERSETUP) == 0) {
			if (!(*tif->tif_setupencode)(tif))
				return (-1);
			tif->tif_flags |= TIFF_CODERSETUP;
		}
        
		tif->tif_rawcc = 0;
		tif->tif_rawcp = tif->tif_rawdata;

		if( td->td_stripbytecount[strip] > 0 )
		{
			 
			td->td_stripbytecount[strip] = 0;

			 
			tif->tif_curoff = 0;
		}

		if (!(*tif->tif_preencode)(tif, sample))
			return (-1);
		tif->tif_flags |= TIFF_POSTENCODE;
	}
	 
	if (row != tif->tif_row) {
		if (row < tif->tif_row) {
			 
			tif->tif_row = (strip % td->td_stripsperimage) *
			    td->td_rowsperstrip;
			tif->tif_rawcp = tif->tif_rawdata;
		}
		 
		if (!(*tif->tif_seek)(tif, row - tif->tif_row))
			return (-1);
		tif->tif_row = row;
	}

	 
	tif->tif_postdecode( tif, (uint8*) buf, tif->tif_scanlinesize );

	status = (*tif->tif_encoderow)(tif, (uint8*) buf,
	    tif->tif_scanlinesize, sample);

         
	tif->tif_row = row + 1;
	return (status);
}
