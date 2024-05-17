static bool extract_delimited_data_as_base64(
	char const *input_buffer,
	int input_len,
	char const *begin_marker,
	char const *end_marker,
	MyString &output_buffer,
	MyString *error_msg)
{
	int start = find_str_in_buffer(input_buffer,input_len,begin_marker);
	int end = find_str_in_buffer(input_buffer,input_len,end_marker);
	if( start < 0 ) {
		if( error_msg ) {
			error_msg->sprintf("Failed to find '%s' in input: %.*s",
							   begin_marker,input_len,input_buffer);
		}
		return false;
	}
	start += strlen(begin_marker);
	if( end < 0 || end < start ) {
		if( error_msg ) {
			error_msg->sprintf("Failed to find '%s' in input: %.*s",
							   end_marker,input_len,input_buffer);
		}
		return false;
	}
	char *encoded = condor_base64_encode((unsigned char const *)input_buffer+start,end-start);
	output_buffer = encoded;
	free(encoded);
	return true;
}
