CreatePixels(
    char		**dataptr,
    unsigned int	  data_size,
    unsigned int	  width,
    unsigned int	  height,
    unsigned int	  cpp,
    unsigned int	 *pixels,
    XpmColor		 *colors)
{
    char *s;
    unsigned int x, y, h, offset;

    if(height <= 1)
    	return;

    h = height - 1;

    offset = width * cpp + 1;

    if(offset <= width || offset <= cpp)
    	return;

     
    for (y = 0; y < h; y++, dataptr++) {
	s = *dataptr;
	 
	for (x = 0; x < width; x++, pixels++) {
	    if(cpp > (data_size - (s - *dataptr)))
	    	return;
	    strncpy(s, colors[*pixels].string, cpp);  
	    s += cpp;
	}
	*s = '\0';
	if(offset > data_size)
		return;
	*(dataptr + 1) = *dataptr + offset;
    }
     
    s = *dataptr;
     
    for (x = 0; x < width; x++, pixels++) {
	if(cpp > data_size - (s - *dataptr))
	    	return;
	strncpy(s, colors[*pixels].string, cpp);  
	s += cpp;
    }
    *s = '\0';
}
