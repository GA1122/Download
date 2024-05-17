static int nextSrcImage (TIFF *tif, char **imageSpec)
 
{
	if (**imageSpec == comma) {   
		char *start = *imageSpec + 1;
		tdir_t nextImage = (tdir_t)strtol(start, imageSpec, 0);
		if (start == *imageSpec) nextImage = TIFFCurrentDirectory (tif);
		if (**imageSpec)
		{
			if (**imageSpec == comma) {
				 
				if ((*imageSpec)[1] == '\0') *imageSpec = NULL;
			}else{
				fprintf (stderr,
				    "Expected a %c separated image # list after %s\n",
				    comma, TIFFFileName (tif));
				exit (-4);    
			}
		}
		if (TIFFSetDirectory (tif, nextImage)) return 1;
		fprintf (stderr, "%s%c%d not found!\n",
		    TIFFFileName(tif), comma, (int) nextImage);
	}
	return 0;
}
