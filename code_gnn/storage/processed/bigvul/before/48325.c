TIFFWriteCheck(TIFF* tif, int tiles, const char* module)
{
	if (tif->tif_mode == O_RDONLY) {
		TIFFErrorExt(tif->tif_clientdata, module, "File not open for writing");
		return (0);
	}
	if (tiles ^ isTiled(tif)) {
		TIFFErrorExt(tif->tif_clientdata, module, tiles ?
		    "Can not write tiles to a stripped image" :
		    "Can not write scanlines to a tiled image");
		return (0);
	}

        _TIFFFillStriles( tif );
        
	 
	if (!TIFFFieldSet(tif, FIELD_IMAGEDIMENSIONS)) {
		TIFFErrorExt(tif->tif_clientdata, module,
		    "Must set \"ImageWidth\" before writing data");
		return (0);
	}
	if (tif->tif_dir.td_samplesperpixel == 1) {
		 
		if (!TIFFFieldSet(tif, FIELD_PLANARCONFIG))
                    tif->tif_dir.td_planarconfig = PLANARCONFIG_CONTIG;
	} else {
		if (!TIFFFieldSet(tif, FIELD_PLANARCONFIG)) {
			TIFFErrorExt(tif->tif_clientdata, module,
			    "Must set \"PlanarConfiguration\" before writing data");
			return (0);
		}
	}
	if (tif->tif_dir.td_stripoffset == NULL && !TIFFSetupStrips(tif)) {
		tif->tif_dir.td_nstrips = 0;
		TIFFErrorExt(tif->tif_clientdata, module, "No space for %s arrays",
		    isTiled(tif) ? "tile" : "strip");
		return (0);
	}
	if (isTiled(tif))
	{
		tif->tif_tilesize = TIFFTileSize(tif);
		if (tif->tif_tilesize == 0)
			return (0);
	}
	else
		tif->tif_tilesize = (tmsize_t)(-1);
	tif->tif_scanlinesize = TIFFScanlineSize(tif);
	if (tif->tif_scanlinesize == 0)
		return (0);
	tif->tif_flags |= TIFF_BEENWRITING;
	return (1);
}
