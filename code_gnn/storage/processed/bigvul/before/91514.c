const char * gdJpegGetVersionString()
{
	switch(JPEG_LIB_VERSION) {
		case 62:
			return "6b";
			break;

		case 70:
			return "7";
			break;

		case 80:
			return "8";
			break;

		case 90:
			return "9 compatible";
			break;

		default:
			return "unknown";
 	}
 }