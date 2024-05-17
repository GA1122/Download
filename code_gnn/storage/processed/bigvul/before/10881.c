_ksba_cert_cmp (ksba_cert_t a, ksba_cert_t b)
{
  const unsigned char *img_a, *img_b;
  size_t len_a, len_b;

  img_a = ksba_cert_get_image (a, &len_a);
  if (!img_a)
    return 1;
  img_b = ksba_cert_get_image (b, &len_b);
  if (!img_b)
    return 1;
  return !(len_a == len_b && !memcmp (img_a, img_b, len_a));
}
