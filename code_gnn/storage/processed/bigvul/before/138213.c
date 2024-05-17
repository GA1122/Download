AXObject::~AXObject() {
  ASSERT(isDetached());
  --s_numberOfLiveAXObjects;
}
