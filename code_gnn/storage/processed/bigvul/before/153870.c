bool AnyBitsSet(GLbitfield bits, GLbitfield ref) {
  DCHECK_NE(0u, ref);
  return ((bits & ref) != 0);
}
