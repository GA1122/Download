_gdScalePass(const gdImagePtr pSrc, const unsigned int src_len,
             const gdImagePtr pDst, const unsigned int dst_len,
             const unsigned int num_lines,
             const gdAxis axis)
{
	unsigned int line_ndx;
	LineContribType * contrib;

     
    assert(dst_len != src_len);  

	contrib = _gdContributionsCalc(dst_len, src_len,
                                   (double)dst_len / (double)src_len,
                                   pSrc->interpolation);
	if (contrib == NULL) {
		return 0;
	}

	 
    for (line_ndx = 0; line_ndx < num_lines; line_ndx++) {
        _gdScaleOneAxis(pSrc, pDst, dst_len, line_ndx, contrib, axis);
	}
	_gdContributionsFree (contrib);

    return 1;
} 
