bool findMetadata(const Metadata::Filter& filter, const int32_t val)
{
 if (filter.isEmpty()) return false;
 if (filter[0] == Metadata::kAny) return true;

 return filter.indexOf(val) >= 0;
}
