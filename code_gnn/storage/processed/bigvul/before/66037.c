static uint8_t* jmp_if(
    int condition,
    uint8_t* ip)
{
  uint8_t* result;

  if (condition)
  {
    result = *(uint8_t**)(ip + 1);


    result--;
  }
  else
  {
    result = ip + sizeof(uint64_t);
  }

  return result;
}
