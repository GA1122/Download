align_column_to_tab (parser_line_counter_t column)  
{
   
  return (parser_line_counter_t) (((column + (8u - 1u)) & ~ECMA_STRING_CONTAINER_MASK) + 1u);
}  
