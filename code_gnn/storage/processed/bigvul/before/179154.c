 void* ipc_alloc(int size)
// void *ipc_alloc(int size)
  {
	void* out;
// 	void *out;
  	if(size > PAGE_SIZE)
  		out = vmalloc(size);
  	else
 		out = kmalloc(size, GFP_KERNEL);
 	return out;
 }