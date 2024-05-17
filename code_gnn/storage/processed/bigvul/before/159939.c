bool ThumbnailsAreEqual(base::RefCountedMemory* t1,
                        base::RefCountedMemory* t2) {
  if (!t1 || !t2)
    return false;
  if (t1->size() != t2->size())
    return false;
  return !memcmp(t1->front(), t2->front(), t1->size());
}
