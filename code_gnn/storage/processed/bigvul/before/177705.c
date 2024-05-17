 int ThreadHook(void* data, void* return_value) {
 int* const hook_data = reinterpret_cast<int*>(data);
 *hook_data = 5;

   return *reinterpret_cast<int*>(return_value);
 }
