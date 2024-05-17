static const FragmentData& FragmentAt(const LayoutObject* obj, unsigned count) {
  auto* fragment = &obj->FirstFragment();
  while (count > 0) {
    count--;
    fragment = fragment->NextFragment();
  }
  return *fragment;
}
