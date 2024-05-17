FaxModem::initializeDecoder(const Class2Params& params)
{
    setupDecoder(recvFillOrder, params.is2D(), (params.df == DF_2DMMR));

    u_int rowpixels = params.pageWidth();	 
    tiff_runlen_t runs[2*4864];			 
    setRuns(runs, runs+4864, rowpixels);
    setIsECM(false);
    resetLineCounts();
}
