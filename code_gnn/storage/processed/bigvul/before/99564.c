const SBFullHash& SBEntry::FullHashAt(int index) const {
  DCHECK(!IsPrefix());

  return IsAdd() ? add_full_hashes_[index] : sub_full_hashes_[index].prefix;
}
