chash_mid(int type, void *base, uschar *string)
{
if (type == HMAC_MD5)
  md5_mid((md5 *)base, string);
else
  sha1_mid((sha1 *)base, string);
}
