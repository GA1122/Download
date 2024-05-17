 XpmCreateDataFromXpmImage(
     char	***data_return,
     XpmImage	  *image,
     XpmInfo	  *info)
 {
      
     int ErrorStatus;
     char buf[BUFSIZ];
     char **header = NULL, **data, **sptr, **sptr2, *s;
     unsigned int header_size, header_nlines;
     unsigned int data_size, data_nlines;
     unsigned int extensions = 0, ext_size = 0, ext_nlines = 0;
     unsigned int offset, l, n;
 
     *data_return = NULL;
 
     extensions = info && (info->valuemask & XpmExtensions)
 	&& info->nextensions;
  
       
      if (extensions)
	CountExtensions(info->extensions, info->nextensions,
			&ext_size, &ext_nlines);
// 	if (CountExtensions(info->extensions, info->nextensions,
// 			&ext_size, &ext_nlines))
// 	    return(XpmNoMemory);
  
       
     header_nlines = 1 + image->ncolors;  
 
      
     if(header_nlines <= image->ncolors ||
        header_nlines >= UINT_MAX / sizeof(char *))
     	return(XpmNoMemory);
 
     header_size = sizeof(char *) * header_nlines;
     if (header_size >= UINT_MAX / sizeof(char *))
 	return (XpmNoMemory);
     header = (char **) XpmCalloc(header_size, sizeof(char *));  
     if (!header)
 	return (XpmNoMemory);
 
      
     s = buf;
 #ifndef VOID_SPRINTF
     s +=
 #endif
     sprintf(s, "%d %d %d %d", image->width, image->height,
 	    image->ncolors, image->cpp);
 #ifdef VOID_SPRINTF
     s += strlen(s);
 #endif
 
     if (info && (info->valuemask & XpmHotspot)) {
 #ifndef VOID_SPRINTF
 	s +=
 #endif
 	sprintf(s, " %d %d", info->x_hotspot, info->y_hotspot);
 #ifdef VOID_SPRINTF
 	s += strlen(s);
 #endif
     }
     if (extensions) {
 	strcpy(s, " XPMEXT");
 	s += 7;
     }
     l = s - buf + 1;
     *header = (char *) XpmMalloc(l);
     if (!*header)
 	RETURN(XpmNoMemory);
     header_size += l;
     strcpy(*header, buf);
 
      
     ErrorStatus = CreateColors(header + 1, &header_size,
 			       image->colorTable, image->ncolors, image->cpp);
 
     if (ErrorStatus != XpmSuccess)
 	RETURN(ErrorStatus);
 
      
     offset = image->width * image->cpp + 1;
 
     if(offset <= image->width || offset <= image->cpp)
      if(offset <= image->width || offset <= image->cpp)
  	RETURN(XpmNoMemory);
  
    if( (image->height + ext_nlines) >= UINT_MAX / sizeof(char *))
//     if (image->height > UINT_MAX - ext_nlines ||
// 	image->height + ext_nlines >= UINT_MAX / sizeof(char *))
  	RETURN(XpmNoMemory);
      data_size = (image->height + ext_nlines) * sizeof(char *);
  
 	RETURN(XpmNoMemory);
     data_size += image->height * offset;
  	RETURN(XpmNoMemory);
      data_size += image->height * offset;
  
    if( (header_size + ext_size) >= (UINT_MAX - data_size) )
//     if (header_size > UINT_MAX - ext_size ||
// 	header_size + ext_size >= (UINT_MAX - data_size) )
  	RETURN(XpmNoMemory);
      data_size += header_size + ext_size;
  
 
     data_nlines = header_nlines + image->height + ext_nlines;
     *data = (char *) (data + data_nlines);
 
      
     n = image->ncolors;
     for (l = 0, sptr = data, sptr2 = header; l <= n && sptr && sptr2; l++, sptr++, sptr2++) {
 	strcpy(*sptr, *sptr2);
 	*(sptr + 1) = *sptr + strlen(*sptr2) + 1;
     }
 
      
     data[header_nlines] = (char *) data + header_size
 	+ (image->height + ext_nlines) * sizeof(char *);
 
     CreatePixels(data + header_nlines, data_size-header_nlines, image->width, image->height,
 		 image->cpp, image->data, image->colorTable);
 
      
     if (extensions)
 	CreateExtensions(data + header_nlines + image->height - 1,
 			 data_size - header_nlines - image->height + 1, offset,
 			 info->extensions, info->nextensions,
 			 ext_nlines);
 
     *data_return = data;
     ErrorStatus = XpmSuccess;
 
  
 exit:
     if (header) {
 	for (l = 0; l < header_nlines; l++)
 	    if (header[l])
 		XpmFree(header[l]);
 		XpmFree(header);
     }
     return(ErrorStatus);
 }