_gdImageGd2 (gdImagePtr im, gdIOCtx * out, int cs, int fmt)
{
	int ncx, ncy, cx, cy;
	int x, y, ylo, yhi, xlo, xhi;
	int chunkLen;
	int chunkNum = 0;
	char *chunkData = NULL;	 
	char *compData = NULL;	 
	uLongf compLen;
	int idxPos = 0;
	int idxSize;
	t_chunk_info *chunkIdx = NULL;
	int posSave;
	int bytesPerPixel = im->trueColor ? 4 : 1;
	int compMax = 0;

	 

	 
	 
	 
	if ((fmt != GD2_FMT_RAW) && (fmt != GD2_FMT_COMPRESSED)) {
		fmt = im->trueColor ? GD2_FMT_TRUECOLOR_COMPRESSED : GD2_FMT_COMPRESSED;
	};
	if (im->trueColor) {
		fmt += 2;
	}
	 
	 
	 
	 
	 
	 
	if (cs == 0) {
		cs = GD2_CHUNKSIZE;
	} else if (cs < GD2_CHUNKSIZE_MIN) {
		cs = GD2_CHUNKSIZE_MIN;
	} else if (cs > GD2_CHUNKSIZE_MAX) {
		cs = GD2_CHUNKSIZE_MAX;
	};

	 
	ncx = im->sx / cs + 1;
	ncy = im->sy / cs + 1;

	 
	_gd2PutHeader (im, out, cs, fmt, ncx, ncy);

	if (gd2_compressed (fmt)) {
		 
		 
		 
		 
		 
		 
		 
		compMax = cs * bytesPerPixel * cs * 1.02 + 12;

		 
		 
		 
		chunkData = gdCalloc (cs * bytesPerPixel * cs, 1);
		if (!chunkData) {
			goto fail;
		}
		compData = gdCalloc (compMax, 1);
		if (!compData) {
			goto fail;
		}

		 
		 
		 
		 
		idxPos = gdTell (out);
		idxSize = ncx * ncy * sizeof (t_chunk_info);
		GD2_DBG (printf ("Index size is %d\n", idxSize));
		gdSeek (out, idxPos + idxSize);

		chunkIdx = gdCalloc (idxSize * sizeof (t_chunk_info), 1);
		if (!chunkIdx) {
			goto fail;
		}
	};

	_gdPutColors (im, out);

	GD2_DBG (printf ("Size: %dx%d\n", im->sx, im->sy));
	GD2_DBG (printf ("Chunks: %dx%d\n", ncx, ncy));

	for (cy = 0; (cy < ncy); cy++) {
		for (cx = 0; (cx < ncx); cx++) {

			ylo = cy * cs;
			yhi = ylo + cs;
			if (yhi > im->sy) {
				yhi = im->sy;
			};

			GD2_DBG (printf
			         ("Processing Chunk (%dx%d), y from %d to %d\n", cx, cy,
			          ylo, yhi));
			chunkLen = 0;
			for (y = ylo; (y < yhi); y++) {

				 

				xlo = cx * cs;
				xhi = xlo + cs;
				if (xhi > im->sx) {
					xhi = im->sx;
				};

				if (gd2_compressed (fmt)) {
					for (x = xlo; x < xhi; x++) {
						 
						 
						if (im->trueColor) {
							int p = im->tpixels[y][x];
							chunkData[chunkLen++] = gdTrueColorGetAlpha (p);
							chunkData[chunkLen++] = gdTrueColorGetRed (p);
							chunkData[chunkLen++] = gdTrueColorGetGreen (p);
							chunkData[chunkLen++] = gdTrueColorGetBlue (p);
						} else {
							int p = im->pixels[y][x];
							chunkData[chunkLen++] = p;
						}
					};
				} else {
					for (x = xlo; x < xhi; x++) {
						 

						if (im->trueColor) {
							gdPutInt (im->tpixels[y][x], out);
						} else {
							gdPutC ((unsigned char) im->pixels[y][x], out);
						}
					};
				};
				 
			};
			if (gd2_compressed (fmt)) {
				compLen = compMax;
				if (compress ((unsigned char *)
				              &compData[0], &compLen,
				              (unsigned char *) &chunkData[0],
				              chunkLen) != Z_OK) {
					printf ("Error from compressing\n");
				} else {
					chunkIdx[chunkNum].offset = gdTell (out);
					chunkIdx[chunkNum++].size = compLen;
					GD2_DBG (printf
					         ("Chunk %d size %d offset %d\n", chunkNum,
					          chunkIdx[chunkNum - 1].size,
					          chunkIdx[chunkNum - 1].offset));

					if (gdPutBuf (compData, compLen, out) <= 0) {
						gd_error("gd write error\n");
					};
				};
			};
		};
	};
	if (gd2_compressed (fmt)) {
		 
		GD2_DBG (printf ("Seeking %d to write index\n", idxPos));
		posSave = gdTell (out);
		gdSeek (out, idxPos);
		GD2_DBG (printf ("Writing index\n"));
		for (x = 0; x < chunkNum; x++) {
			GD2_DBG (printf
			         ("Chunk %d size %d offset %d\n", x, chunkIdx[x].size,
			          chunkIdx[x].offset));
			gdPutInt (chunkIdx[x].offset, out);
			gdPutInt (chunkIdx[x].size, out);
		};
		 
		 
		gdSeek (out, posSave);
	};

	 
fail:
	GD2_DBG (printf ("Freeing memory\n"));

	if (chunkData) {
		gdFree (chunkData);
	}
	if (compData) {
		gdFree (compData);
	}
	if (chunkIdx) {
		gdFree (chunkIdx);
	}
	GD2_DBG (printf ("Done\n"));

}
