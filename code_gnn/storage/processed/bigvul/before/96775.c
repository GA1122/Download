selftest_fips_128 (int extended, selftest_report_func_t report)
{
  const char *what;
  const char *errtxt;

  what = "low-level";
  errtxt = selftest_basic_128 ();
  if (errtxt)
    goto failed;

  if (extended)
    {
      what = "cfb";
      errtxt = selftest_fips_128_38a (GCRY_CIPHER_MODE_CFB);
      if (errtxt)
        goto failed;

      what = "ofb";
      errtxt = selftest_fips_128_38a (GCRY_CIPHER_MODE_OFB);
      if (errtxt)
        goto failed;
    }

  return 0;  

 failed:
  if (report)
    report ("cipher", GCRY_CIPHER_AES128, what, errtxt);
  return GPG_ERR_SELFTEST_FAILED;
}