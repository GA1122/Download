static bool willBeCleared(const GraphicsControlBlock& gcb) {
 return gcb.DisposalMode == DISPOSE_BACKGROUND || gcb.DisposalMode == DISPOSE_PREVIOUS;
}
