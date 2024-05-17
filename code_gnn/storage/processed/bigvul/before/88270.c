generate_hash_secret_salt(XML_Parser parser) {
  unsigned long entropy;
  (void)parser;

   
#if defined(HAVE_ARC4RANDOM_BUF)
  arc4random_buf(&entropy, sizeof(entropy));
  return ENTROPY_DEBUG("arc4random_buf", entropy);
#elif defined(HAVE_ARC4RANDOM)
  writeRandomBytes_arc4random((void *)&entropy, sizeof(entropy));
  return ENTROPY_DEBUG("arc4random", entropy);
#else
   
#  ifdef _WIN32
  if (writeRandomBytes_rand_s((void *)&entropy, sizeof(entropy))) {
    return ENTROPY_DEBUG("rand_s", entropy);
  }
#  elif defined(HAVE_GETRANDOM) || defined(HAVE_SYSCALL_GETRANDOM)
  if (writeRandomBytes_getrandom_nonblock((void *)&entropy, sizeof(entropy))) {
    return ENTROPY_DEBUG("getrandom", entropy);
  }
#  endif
#  if ! defined(_WIN32) && defined(XML_DEV_URANDOM)
  if (writeRandomBytes_dev_urandom((void *)&entropy, sizeof(entropy))) {
    return ENTROPY_DEBUG("/dev/urandom", entropy);
  }
#  endif  
   

   
  entropy = gather_time_entropy() ^ getpid();

   
  if (sizeof(unsigned long) == 4) {
    return ENTROPY_DEBUG("fallback(4)", entropy * 2147483647);
  } else {
    return ENTROPY_DEBUG("fallback(8)",
                         entropy * (unsigned long)2305843009213693951ULL);
  }
#endif
}
