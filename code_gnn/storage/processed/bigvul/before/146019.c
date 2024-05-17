GLsync SyncObjectOrZero(const WebGLSync* object) {
  return object ? object->Object() : nullptr;
}
