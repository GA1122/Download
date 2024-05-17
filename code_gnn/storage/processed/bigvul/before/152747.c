bool Histogram::SerializeInfoImpl(Pickle* pickle) const {
  DCHECK(bucket_ranges()->HasValidChecksum());
  return pickle->WriteString(histogram_name()) &&
      pickle->WriteInt(flags()) &&
      pickle->WriteInt(declared_min()) &&
      pickle->WriteInt(declared_max()) &&
      pickle->WriteUInt32(bucket_count()) &&
      pickle->WriteUInt32(bucket_ranges()->checksum());
}
