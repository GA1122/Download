TIFFWriteEncodedStrip(TIFF* tif, uint32 strip, void* data, tmsize_t cc)
{
	static const char module[] = "TIFFWriteEncodedStrip";
	TIFFDirectory *td = &tif->tif_dir;
	uint16 sample;

	if (!WRITECHECKSTRIPS(tif, module))
		return ((tmsize_t) -1);
	 
	if (strip >= td->td_nstrips) {
		if (td->td_planarconfig == PLANARCONFIG_SEPARATE) {
			TIFFErrorExt(tif->tif_clientdata, module,
			    "Can not grow image by strips when using separate planes");
			return ((tmsize_t) -1);
		}
		if (!TIFFGrowStrips(tif, 1, module))
			return ((tmsize_t) -1);
		td->td_stripsperimage =
		    TIFFhowmany_32(td->td_imagelength, td->td_rowsperstrip);  
	}
	 
	if (!BUFFERCHECK(tif))
		return ((tmsize_t) -1);

        tif->tif_flags |= TIFF_BUF4WRITE;
	tif->tif_curstrip = strip;

        if (td->td_stripsperimage == 0) {
                TIFFErrorExt(tif->tif_clientdata, module, "Zero strips per image");
                return ((tmsize_t) -1);
        }

	tif->tif_row = (strip % td->td_stripsperimage) * td->td_rowsperstrip;
	if ((tif->tif_flags & TIFF_CODERSETUP) == 0) {
		if (!(*tif->tif_setupencode)(tif))
			return ((tmsize_t) -1);
		tif->tif_flags |= TIFF_CODERSETUP;
	}

	if( td->td_stripbytecount[strip] > 0 )
        {
             
             
             
             
            if( tif->tif_rawdatasize <= (tmsize_t)td->td_stripbytecount[strip] )
            {
                if( !(TIFFWriteBufferSetup(tif, NULL,
                    (tmsize_t)TIFFroundup_64((uint64)(td->td_stripbytecount[strip] + 1), 1024))) )
                    return ((tmsize_t)(-1));
            }

	     
            tif->tif_curoff = 0;
        }

    tif->tif_rawcc = 0;
    tif->tif_rawcp = tif->tif_rawdata;

	tif->tif_flags &= ~TIFF_POSTENCODE;

     
    if( td->td_compression == COMPRESSION_NONE )
    {
         
        tif->tif_postdecode( tif, (uint8*) data, cc );

        if (!isFillOrder(tif, td->td_fillorder) &&
            (tif->tif_flags & TIFF_NOBITREV) == 0)
            TIFFReverseBits((uint8*) data, cc);

        if (cc > 0 &&
            !TIFFAppendToStrip(tif, strip, (uint8*) data, cc))
            return ((tmsize_t) -1);
        return (cc);
    }

	sample = (uint16)(strip / td->td_stripsperimage);
	if (!(*tif->tif_preencode)(tif, sample))
		return ((tmsize_t) -1);

         
	tif->tif_postdecode( tif, (uint8*) data, cc );

	if (!(*tif->tif_encodestrip)(tif, (uint8*) data, cc, sample))
		return ((tmsize_t) -1);
	if (!(*tif->tif_postencode)(tif))
		return ((tmsize_t) -1);
	if (!isFillOrder(tif, td->td_fillorder) &&
	    (tif->tif_flags & TIFF_NOBITREV) == 0)
		TIFFReverseBits(tif->tif_rawdata, tif->tif_rawcc);
	if (tif->tif_rawcc > 0 &&
	    !TIFFAppendToStrip(tif, strip, tif->tif_rawdata, tif->tif_rawcc))
		return ((tmsize_t) -1);
	tif->tif_rawcc = 0;
	tif->tif_rawcp = tif->tif_rawdata;
	return (cc);
}
