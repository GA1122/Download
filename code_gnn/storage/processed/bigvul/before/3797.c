_dbus_get_real_time (long *tv_sec,
                     long *tv_usec)
{
  FILETIME ft;
  dbus_uint64_t time64;

  GetSystemTimeAsFileTime (&ft);

  memcpy (&time64, &ft, sizeof (time64));

   
  time64 -= DBUS_INT64_CONSTANT (116444736000000000);
  time64 /= 10;

  if (tv_sec)
    *tv_sec = time64 / 1000000;

  if (tv_usec)
    *tv_usec = time64 % 1000000;
}
