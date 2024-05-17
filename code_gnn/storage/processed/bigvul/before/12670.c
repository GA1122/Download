prvs_hmac_sha1(uschar *address, uschar *key, uschar *key_num, uschar *daystamp)
{
uschar *hash_source, *p;
int size = 0,offset = 0,i;
sha1 sha1_base;
void *use_base = &sha1_base;
uschar innerhash[20];
uschar finalhash[20];
uschar innerkey[64];
uschar outerkey[64];
uschar *finalhash_hex = store_get(40);

if (key_num == NULL)
  key_num = US"0";

if (Ustrlen(key) > 64)
  return NULL;

hash_source = string_cat(NULL,&size,&offset,key_num,1);
string_cat(hash_source,&size,&offset,daystamp,3);
string_cat(hash_source,&size,&offset,address,Ustrlen(address));
hash_source[offset] = '\0';

DEBUG(D_expand) debug_printf("prvs: hash source is '%s'\n", hash_source);

memset(innerkey, 0x36, 64);
memset(outerkey, 0x5c, 64);

for (i = 0; i < Ustrlen(key); i++)
  {
  innerkey[i] ^= key[i];
  outerkey[i] ^= key[i];
  }

chash_start(HMAC_SHA1, use_base);
chash_mid(HMAC_SHA1, use_base, innerkey);
chash_end(HMAC_SHA1, use_base, hash_source, offset, innerhash);

chash_start(HMAC_SHA1, use_base);
chash_mid(HMAC_SHA1, use_base, outerkey);
chash_end(HMAC_SHA1, use_base, innerhash, 20, finalhash);

p = finalhash_hex;
for (i = 0; i < 3; i++)
  {
  *p++ = hex_digits[(finalhash[i] & 0xf0) >> 4];
  *p++ = hex_digits[finalhash[i] & 0x0f];
  }
*p = '\0';

return finalhash_hex;
}
