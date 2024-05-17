static TIFF* openSrcImage (char **imageSpec)
 
{
	TIFF *tif;
	char *fn = *imageSpec;
	*imageSpec = strchr (fn, comma);
	if (*imageSpec) {   
		**imageSpec = '\0';
		tif = TIFFOpen (fn, "r");
		 
		if (!(*imageSpec)[1]) {*imageSpec = NULL; return tif;}
		if (tif) {
			**imageSpec = comma;   
			if (!nextSrcImage(tif, imageSpec)) {
				TIFFClose (tif);
				tif = NULL;
			}
		}
	}else
		tif = TIFFOpen (fn, "r");
	return tif;
}
