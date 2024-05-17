selftest_fips_256 (int extended, selftest_report_func_t report)
{
  const char *what;
  const char *errtxt;

  (void)extended;  

  what = "low-level";
  errtxt = selftest_basic_256 ();
  if (errtxt)
    goto failed;

  return 0;  

 failed:
  if (report)
    report ("cipher", GCRY_CIPHER_AES256, what, errtxt);
  return GPG_ERR_SELFTEST_FAILED;
}