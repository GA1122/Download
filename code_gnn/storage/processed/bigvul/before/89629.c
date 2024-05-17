is_tag_length_valid(size_t taglen)
{
  switch (taglen)
    {
     
    case 128 / 8:  
    case 120 / 8:
    case 112 / 8:
    case 104 / 8:
    case 96 / 8:
    case 64 / 8:
    case 32 / 8:
      return 1;

    default:
      return 0;
    }
}
