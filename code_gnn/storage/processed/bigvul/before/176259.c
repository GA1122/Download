 static uint32_t FilterKey(Handle<SeededNumberDictionary> dictionary,
 int entry, Object* raw_key, PropertyFilter filter) {
    DCHECK(!dictionary->IsDeleted(entry));
    DCHECK(raw_key->IsNumber());
    DCHECK_LE(raw_key->Number(), kMaxUInt32);
 PropertyDetails details = dictionary->DetailsAt(entry);
 PropertyAttributes attr = details.attributes();
 if ((attr & filter) != 0) return kMaxUInt32;
 return static_cast<uint32_t>(raw_key->Number());
 }
