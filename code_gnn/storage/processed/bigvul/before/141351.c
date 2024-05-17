const AtomicString& Document::referrer() const {
  if (Loader())
    return Loader()->GetReferrer().referrer;
  return g_null_atom;
}
