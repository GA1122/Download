PixarLogCleanup(TIFF* tif)
{
	PixarLogState* sp = (PixarLogState*) tif->tif_data;

	assert(sp != 0);

	(void)TIFFPredictorCleanup(tif);

	tif->tif_tagmethods.vgetfield = sp->vgetparent;
	tif->tif_tagmethods.vsetfield = sp->vsetparent;

	if (sp->FromLT2) _TIFFfree(sp->FromLT2);
	if (sp->From14) _TIFFfree(sp->From14);
	if (sp->From8) _TIFFfree(sp->From8);
	if (sp->ToLinearF) _TIFFfree(sp->ToLinearF);
	if (sp->ToLinear16) _TIFFfree(sp->ToLinear16);
	if (sp->ToLinear8) _TIFFfree(sp->ToLinear8);
	if (sp->state&PLSTATE_INIT) {
		if (tif->tif_mode == O_RDONLY)
			inflateEnd(&sp->stream);
		else
			deflateEnd(&sp->stream);
	}
	if (sp->tbuf)
		_TIFFfree(sp->tbuf);
	_TIFFfree(sp);
	tif->tif_data = NULL;

	_TIFFSetDefaultCompressionState(tif);
}