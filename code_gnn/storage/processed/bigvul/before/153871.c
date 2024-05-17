bool AnyOtherBitsSet(GLbitfield bits, GLbitfield ref) {
  DCHECK_NE(0u, ref);
  GLbitfield mask = ~ref;
  return ((bits & mask) != 0);
}
