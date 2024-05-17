static void pop_string(stroke_msg_t *msg, char **string)
{
	if (*string == NULL)
	{
		return;
	}

	 
	if (string < (char**)msg ||
		string > (char**)((char*)msg + sizeof(stroke_msg_t)) ||
		(unsigned long)*string < (unsigned long)((char*)msg->buffer - (char*)msg) ||
		(unsigned long)*string > msg->length)
	{
		*string = "(invalid pointer in stroke msg)";
	}
	else
	{
		*string = (char*)msg + (unsigned long)*string;
	}
}
