static int get_appcontext_input_count(void)
{
  static int input_count_offset = -1;
  if (input_count_offset < 0)
	input_count_offset = get_appcontext_input_count_offset();
  if (input_count_offset == 0)
	return 1;  
  return get_appcontext_input_count_at(input_count_offset);
}
