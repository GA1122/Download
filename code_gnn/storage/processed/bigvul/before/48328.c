TIFFWriteRawStrip(TIFF* tif, uint32 strip, void* data, tmsize_t cc)
{
	static const char module[] = "TIFFWriteRawStrip";
	TIFFDirectory *td = &tif->tif_dir;

	if (!WRITECHECKSTRIPS(tif, module))
		return ((tmsize_t) -1);
	 
	if (strip >= td->td_nstrips) {
		if (td->td_planarconfig == PLANARCONFIG_SEPARATE) {
			TIFFErrorExt(tif->tif_clientdata, module,
			    "Can not grow image by strips when using separate planes");
			return ((tmsize_t) -1);
		}
		 
		if (strip >= td->td_stripsperimage)
			td->td_stripsperimage =
			    TIFFhowmany_32(td->td_imagelength,td->td_rowsperstrip);
		if (!TIFFGrowStrips(tif, 1, module))
			return ((tmsize_t) -1);
	}
	tif->tif_curstrip = strip;
        if (td->td_stripsperimage == 0) {
                TIFFErrorExt(tif->tif_clientdata, module,"Zero strips per image");
                return ((tmsize_t) -1);
        }
	tif->tif_row = (strip % td->td_stripsperimage) * td->td_rowsperstrip;
	return (TIFFAppendToStrip(tif, strip, (uint8*) data, cc) ?
	    cc : (tmsize_t) -1);
}
