void SBEntry::SetPrefixAt(int index, const SBPrefix& prefix) {
  DCHECK(IsPrefix());

  if (IsAdd())
    add_prefixes_[index] = prefix;
  else
    sub_prefixes_[index].prefix = prefix;
}
