BGD_DECLARE(gdImagePtr) gdImageCreateFromGd2PartCtx (gdIOCtx * in, int srcx, int srcy, int w, int h)
{
	int scx, scy, ecx, ecy, fsx, fsy;
	int nc, ncx, ncy, cs, cx, cy;
	int x, y, ylo, yhi, xlo, xhi;
	int dstart, dpos;
	int i;
	 
	unsigned int ch;
	int vers, fmt;
	t_chunk_info *chunkIdx = NULL;
	unsigned char *chunkBuf = NULL;
	int chunkNum;
	int chunkMax = 0;
	uLongf chunkLen;
	int chunkPos = 0;
	int compMax;
	char *compBuf = NULL;

	gdImagePtr im;

	 
	 
	 
	 
	 
	if (_gd2GetHeader (in, &fsx, &fsy, &cs, &vers, &fmt, &ncx, &ncy, &chunkIdx)
	        != 1) {
		goto fail1;
	}

	GD2_DBG (printf ("File size is %dx%d\n", fsx, fsy));

	 
	if (gd2_truecolor (fmt)) {
		im = gdImageCreateTrueColor (w, h);
	} else {
		im = gdImageCreate (w, h);
	}
	if (im == NULL) {
		goto fail1;
	};

	if (!_gdGetColors (in, im, vers == 2)) {
		goto fail2;
	}
	GD2_DBG (printf ("Image palette completed: %d colours\n", im->colorsTotal));

	 
	nc = ncx * ncy;

	if (gd2_compressed (fmt)) {
		 
		compMax = 0;
		for (i = 0; (i < nc); i++) {
			if (chunkIdx[i].size > compMax) {
				compMax = chunkIdx[i].size;
			};
		};
		compMax++;

		if (im->trueColor) {
			chunkMax = cs * cs * 4;
		} else {
			chunkMax = cs * cs;
		}
		chunkBuf = gdCalloc (chunkMax, 1);
		if (!chunkBuf) {
			goto fail2;
		}
		compBuf = gdCalloc (compMax, 1);
		if (!compBuf) {
			goto fail2;
		}

	};

	 
	 
	 
	 


	 
	scx = srcx / cs;
	scy = srcy / cs;
	if (scx < 0) {
		scx = 0;
	};
	if (scy < 0) {
		scy = 0;
	};

	ecx = (srcx + w) / cs;
	ecy = (srcy + h) / cs;
	if (ecx >= ncx) {
		ecx = ncx - 1;
	};
	if (ecy >= ncy) {
		ecy = ncy - 1;
	};

	 
	dstart = gdTell (in);
	GD2_DBG (printf ("Data starts at %d\n", dstart));

	 
	for (cy = scy; (cy <= ecy); cy++) {

		ylo = cy * cs;
		yhi = ylo + cs;
		if (yhi > fsy) {
			yhi = fsy;
		};

		for (cx = scx; (cx <= ecx); cx++) {

			xlo = cx * cs;
			xhi = xlo + cs;
			if (xhi > fsx) {
				xhi = fsx;
			};

			GD2_DBG (printf
			         ("Processing Chunk (%d, %d), from %d to %d\n", cx, cy, ylo,
			          yhi));

			if (!gd2_compressed (fmt)) {
				GD2_DBG (printf ("Using raw format data\n"));
				if (im->trueColor) {
					dpos =
					    (cy * (cs * fsx) * 4 + cx * cs * (yhi - ylo) * 4) +
					    dstart;
				} else {
					dpos = cy * (cs * fsx) + cx * cs * (yhi - ylo) + dstart;
				}
				 
				if (!gdSeek (in, dpos)) {
					gd_error("Seek error\n");
					goto fail2;
				};
				GD2_DBG (printf
				         ("Reading (%d, %d) from position %d\n", cx, cy,
				          dpos - dstart));
			} else {
				chunkNum = cx + cy * ncx;

				chunkLen = chunkMax;
				if (!_gd2ReadChunk (chunkIdx[chunkNum].offset,
				                    compBuf,
				                    chunkIdx[chunkNum].size,
				                    (char *) chunkBuf, &chunkLen, in)) {
					printf ("Error reading comproessed chunk\n");
					goto fail2;
				};
				chunkPos = 0;
				GD2_DBG (printf
				         ("Reading (%d, %d) from chunk %d\n", cx, cy,
				          chunkNum));
			};

			GD2_DBG (printf
			         ("   into (%d, %d) - (%d, %d)\n", xlo, ylo, xhi, yhi));
			for (y = ylo; (y < yhi); y++) {

				for (x = xlo; x < xhi; x++) {
					if (!gd2_compressed (fmt)) {
						if (im->trueColor) {
							if (!gdGetInt ((int *) &ch, in)) {
								ch = 0;
								 
								 
							}
						} else {
							ch = gdGetC (in);
							if ((int) ch == EOF) {
								ch = 0;
								 
								 
							}
						}
					} else {
						if (im->trueColor) {
							ch = chunkBuf[chunkPos++];
							ch = (ch << 8) + chunkBuf[chunkPos++];
							ch = (ch << 8) + chunkBuf[chunkPos++];
							ch = (ch << 8) + chunkBuf[chunkPos++];
						} else {
							ch = chunkBuf[chunkPos++];
						}
					};

					 
					if ((x >= srcx) && (x < (srcx + w)) && (x < fsx) && (x >= 0)
					        && (y >= srcy) && (y < (srcy + h)) && (y < fsy)
					        && (y >= 0)) {
						 
						if (im->trueColor) {
							im->tpixels[y - srcy][x - srcx] = ch;
						} else {
							im->pixels[y - srcy][x - srcx] = ch;
						}
					}
				};
			};
		};
	};

	gdFree (chunkBuf);
	gdFree (compBuf);
	gdFree (chunkIdx);

	return im;

fail2:
	gdImageDestroy (im);
fail1:
	if (chunkBuf) {
		gdFree (chunkBuf);
	}
	if (compBuf) {
		gdFree (compBuf);
	}
	if (chunkIdx) {
		gdFree (chunkIdx);
	}
	return 0;

}