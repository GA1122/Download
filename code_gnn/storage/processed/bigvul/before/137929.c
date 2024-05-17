void AXLayoutObject::detachRemoteSVGRoot() {
  if (AXSVGRoot* root = remoteSVGRootElement())
    root->setParent(0);
}
