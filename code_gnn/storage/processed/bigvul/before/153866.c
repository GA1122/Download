bool AllBitsSet(GLbitfield bits, GLbitfield ref) {
  DCHECK_NE(0u, ref);
  return ((bits & ref) == ref);
}
