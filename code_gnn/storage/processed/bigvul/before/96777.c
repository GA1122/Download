selftest_fips_192 (int extended, selftest_report_func_t report)
{
  const char *what;
  const char *errtxt;

  (void)extended;  

  what = "low-level";
  errtxt = selftest_basic_192 ();
  if (errtxt)
    goto failed;


  return 0;  

 failed:
  if (report)
    report ("cipher", GCRY_CIPHER_AES192, what, errtxt);
  return GPG_ERR_SELFTEST_FAILED;
}
