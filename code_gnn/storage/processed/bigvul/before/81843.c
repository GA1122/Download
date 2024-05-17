int mp_jacobi(mp_int* a, mp_int* n, int* c)
{
    mp_int   a1, n1;
    int      res;

     
    if ((res = mp_init_multi(&a1, &n1, NULL, NULL, NULL, NULL)) != MP_OKAY) {
        return res;
    }

    if ((res = mp_copy(a, &a1)) != MP_OKAY) {
        goto done;
    }

    if ((res = mp_copy(n, &n1)) != MP_OKAY) {
        goto done;
    }

    res = do_mp_jacobi(&a1, &n1, c);

done:
   
  mp_clear(&n1);
  mp_clear(&a1);

  return res;
}