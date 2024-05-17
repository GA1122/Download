static void DuplicateMedium(CLIPFORMAT source_clipformat,
                            STGMEDIUM* source,
                            STGMEDIUM* destination) {
  switch (source->tymed) {
    case TYMED_HGLOBAL:
      destination->hGlobal =
          static_cast<HGLOBAL>(OleDuplicateData(
              source->hGlobal, source_clipformat, 0));
      break;
    case TYMED_MFPICT:
      destination->hMetaFilePict =
          static_cast<HMETAFILEPICT>(OleDuplicateData(
              source->hMetaFilePict, source_clipformat, 0));
      break;
    case TYMED_GDI:
      destination->hBitmap =
          static_cast<HBITMAP>(OleDuplicateData(
              source->hBitmap, source_clipformat, 0));
      break;
    case TYMED_ENHMF:
      destination->hEnhMetaFile =
          static_cast<HENHMETAFILE>(OleDuplicateData(
              source->hEnhMetaFile, source_clipformat, 0));
      break;
    case TYMED_FILE:
      destination->lpszFileName =
          static_cast<LPOLESTR>(OleDuplicateData(
              source->lpszFileName, source_clipformat, 0));
      break;
    case TYMED_ISTREAM:
      destination->pstm = source->pstm;
      destination->pstm->AddRef();
      break;
    case TYMED_ISTORAGE:
      destination->pstg = source->pstg;
      destination->pstg->AddRef();
      break;
  }

  destination->tymed = source->tymed;
  destination->pUnkForRelease = source->pUnkForRelease;
  if (destination->pUnkForRelease)
    destination->pUnkForRelease->AddRef();
}
