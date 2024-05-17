SnapshotReason WebGLRenderingContextBase::FunctionIDToSnapshotReason(
    TexImageFunctionID id) {
  switch (id) {
    case kTexImage2D:
      return kSnapshotReasonWebGLTexImage2D;
    case kTexSubImage2D:
      return kSnapshotReasonWebGLTexSubImage2D;
    case kTexImage3D:
      return kSnapshotReasonWebGLTexImage3D;
    case kTexSubImage3D:
      return kSnapshotReasonWebGLTexSubImage3D;
  }
  NOTREACHED();
  return kSnapshotReasonUnknown;
}
