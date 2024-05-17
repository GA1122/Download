static size_t searchCodeIndex(const unsigned* array, size_t array_size, size_t value)
{
   
   

   
  size_t left  = 1;
  size_t right = array_size - 1;
  while(left <= right)
  {
    size_t mid = (left + right) / 2;
    if(array[mid] <= value) left = mid + 1;  
    else if(array[mid - 1] > value) right = mid - 1;  
    else return mid - 1;
  }
  return array_size - 1;
}
