static void setup_free(vorb *f, void *p)
{
   if (f->alloc.alloc_buffer) return;  
   free(p);
}
