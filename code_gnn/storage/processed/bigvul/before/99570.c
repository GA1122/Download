const SBPrefix& SBEntry::PrefixAt(int index) const {
  DCHECK(IsPrefix());

  return IsAdd() ? add_prefixes_[index] : sub_prefixes_[index].prefix;
}
