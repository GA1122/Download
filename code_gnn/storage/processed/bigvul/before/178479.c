 static void spl_ptr_heap_insert(spl_ptr_heap *heap, spl_ptr_heap_element elem, void *cmp_userdata TSRMLS_DC) {  
 	int i;
 
 	if (heap->count+1 > heap->max_size) {
 		 
 		heap->elements  = (void **) safe_erealloc(heap->elements, sizeof(spl_ptr_heap_element), (heap->max_size), (sizeof(spl_ptr_heap_element) * (heap->max_size)));
 		heap->max_size *= 2;
 	}
 
         heap->ctor(elem TSRMLS_CC);
  
          
       for(i = heap->count++; i > 0 && heap->cmp(heap->elements[(i-1)/2], elem, cmp_userdata TSRMLS_CC) < 0; i = (i-1)/2) {
//        for(i = heap->count; i > 0 && heap->cmp(heap->elements[(i-1)/2], elem, cmp_userdata TSRMLS_CC) < 0; i = (i-1)/2) {
                 heap->elements[i] = heap->elements[(i-1)/2];
         }
//        heap->count++;
  
         if (EG(exception)) {
                  
 	}
 
 	heap->elements[i] = elem;
 
 }
  