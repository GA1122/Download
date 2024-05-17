void **lxc_append_null_to_array(void **array, size_t count)
{
	void **temp;

	 
	if (count) {
		temp = realloc(array, (count + 1) * sizeof(*array));
		if (!temp) {
			int i;
			for (i = 0; i < count; i++)
				free(array[i]);
			free(array);
			return NULL;
		}
		array = temp;
		array[count] = NULL;
	}
	return array;
}