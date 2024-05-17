void wc_ecc_del_point_h(ecc_point* p, void* heap)
{
    
   if (p != NULL) {
      mp_clear(p->x);
      mp_clear(p->y);
      mp_clear(p->z);
      XFREE(p, heap, DYNAMIC_TYPE_ECC);
   }
   (void)heap;
}