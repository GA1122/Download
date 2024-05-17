struct svcinfo *find_svc(const uint16_t *s16, size_t len)
{
 struct svcinfo *si;

 for (si = svclist; si; si = si->next) {
 if ((len == si->len) &&
 !memcmp(s16, si->name, len * sizeof(uint16_t))) {
 return si;
 }
 }
 return NULL;
}
