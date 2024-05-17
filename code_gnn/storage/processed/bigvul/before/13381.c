XpmCreateDataFromImage(
    Display		  *display,
    char		***data_return,
    XImage		  *image,
    XImage		  *shapeimage,
    XpmAttributes	  *attributes)
{
    XpmImage xpmimage;
    XpmInfo info;
    int ErrorStatus;

     
    if (data_return)
	*data_return = NULL;

     
    ErrorStatus = XpmCreateXpmImageFromImage(display, image, shapeimage,
					     &xpmimage, attributes);
    if (ErrorStatus != XpmSuccess)
	return (ErrorStatus);

     
    if (attributes) {
	xpmSetInfo(&info, attributes);
	ErrorStatus = XpmCreateDataFromXpmImage(data_return, &xpmimage, &info);
    } else
	ErrorStatus = XpmCreateDataFromXpmImage(data_return, &xpmimage, NULL);

     
    XpmFreeXpmImage(&xpmimage);

    return (ErrorStatus);
}
