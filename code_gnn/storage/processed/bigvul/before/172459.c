int venc_dev::venc_set_format(int format)
{
 int rc = true;

 if (format)
        color_format = format;
 else {
        color_format = 0;
        rc = false;
 }

 return rc;
}
