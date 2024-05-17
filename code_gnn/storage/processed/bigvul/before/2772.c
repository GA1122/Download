FaxModem::recvEndPage(TIFF* tif, const Class2Params& params)
{
     
#ifdef TIFFTAG_FAXRECVPARAMS
    TIFFSetField(tif, TIFFTAG_FAXRECVPARAMS,	(uint32) params.encode());
#endif
#ifdef TIFFTAG_FAXDCS
    FaxParams pageparams = FaxParams(params);
    fxStr faxdcs = "";
    pageparams.asciiEncode(faxdcs);
    TIFFSetField(tif, TIFFTAG_FAXDCS, (const char*) faxdcs);
#endif
#ifdef TIFFTAG_FAXSUBADDRESS
    if (sub != "")
	TIFFSetField(tif, TIFFTAG_FAXSUBADDRESS,	(const char*) sub);
#endif
#ifdef TIFFTAG_FAXRECVTIME
    TIFFSetField(tif, TIFFTAG_FAXRECVTIME,
	(uint32) server.setPageTransferTime());
#endif
}
