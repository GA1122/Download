status_t QueryCodecs(
 const sp<IOMX> &omx,
 const char *mime, bool queryDecoders, bool hwCodecOnly,
 Vector<CodecCapabilities> *results) {
 Vector<OMXCodec::CodecNameAndQuirks> matchingCodecs;
    results->clear();

 OMXCodec::findMatchingCodecs(mime,
 !queryDecoders  ,
            NULL  ,
            hwCodecOnly ? OMXCodec::kHardwareCodecsOnly : 0  ,
 &matchingCodecs);

 for (size_t c = 0; c < matchingCodecs.size(); c++) {
 const char *componentName = matchingCodecs.itemAt(c).mName.string();

        results->push();
 CodecCapabilities *caps = &results->editItemAt(results->size() - 1);

 status_t err =
 QueryCodec(omx, componentName, mime, !queryDecoders, caps);

 if (err != OK) {
            results->removeAt(results->size() - 1);
 }
 }

 return OK;
}
