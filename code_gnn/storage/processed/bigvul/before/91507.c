 gdImagePtr gdImageCreateFromJpeg (FILE * inFile)
{
	return gdImageCreateFromJpegEx(inFile, 1);
}