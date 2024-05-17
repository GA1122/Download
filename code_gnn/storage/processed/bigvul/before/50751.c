static void network_init_gcrypt (void)  
{
   
  if (gcry_control (GCRYCTL_ANY_INITIALIZATION_P))
    return;

  
# if GCRYPT_VERSION_NUMBER < 0x010600
  gcry_control (GCRYCTL_SET_THREAD_CBS, &gcry_threads_pthread);
# endif
  gcry_check_version (NULL);
  gcry_control (GCRYCTL_INIT_SECMEM, 32768);
  gcry_control (GCRYCTL_INITIALIZATION_FINISHED);
}  
