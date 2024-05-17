chash_end(int type, void *base, uschar *string, int length, uschar *digest)
{
if (type == HMAC_MD5)
  md5_end((md5 *)base, string, length, digest);
else
  sha1_end((sha1 *)base, string, length, digest);
}
