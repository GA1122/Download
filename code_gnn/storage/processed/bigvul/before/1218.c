PSOutputDev::PSOutputDev(PSOutputFunc outputFuncA, void *outputStreamA,
			 char *psTitle,
			 XRef *xrefA, Catalog *catalog,
			 int firstPage, int lastPage, PSOutMode modeA,
			 int paperWidthA, int paperHeightA, GBool duplexA,
			 int imgLLXA, int imgLLYA, int imgURXA, int imgURYA,
			 GBool forceRasterizeA,
			 GBool manualCtrlA) {
  underlayCbk = NULL;
  underlayCbkData = NULL;
  overlayCbk = NULL;
  overlayCbkData = NULL;

  fontIDs = NULL;
  fontFileIDs = NULL;
  fontFileNames = NULL;
  font8Info = NULL;
  font16Enc = NULL;
  imgIDs = NULL;
  formIDs = NULL;
  xobjStack = NULL;
  embFontList = NULL;
  customColors = NULL;
  haveTextClip = gFalse;
  t3String = NULL;

  forceRasterize = forceRasterizeA;

  init(outputFuncA, outputStreamA, psGeneric, psTitle,
       xrefA, catalog, firstPage, lastPage, modeA,
       imgLLXA, imgLLYA, imgURXA, imgURYA, manualCtrlA,
       paperWidthA, paperHeightA, duplexA);
}
