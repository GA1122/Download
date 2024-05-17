void SoftMPEG4::updatePortDefinitions(bool updateCrop, bool updateInputSize) {
 SoftVideoDecoderOMXComponent::updatePortDefinitions(updateCrop, updateInputSize);

  
    OMX_PARAM_PORTDEFINITIONTYPE *def = &editPortInfo(kOutputPortIndex)->mDef;
    def->format.video.nStride = align(def->format.video.nStride, 16);
    def->format.video.nSliceHeight = align(def->format.video.nSliceHeight, 16);
    def->nBufferSize = (def->format.video.nStride * def->format.video.nSliceHeight * 3) / 2;
}
