static void CloneFormatEtc(FORMATETC* source, FORMATETC* clone) {
  *clone = *source;
  if (source->ptd) {
    source->ptd =
        static_cast<DVTARGETDEVICE*>(CoTaskMemAlloc(sizeof(DVTARGETDEVICE)));
    *(clone->ptd) = *(source->ptd);
  }
}
