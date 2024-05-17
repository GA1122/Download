void H264SwDecTrace(char *string)
{
 FILE *fp;

    fp = fopen("dec_api.trc", "at");

 if (!fp)
 return;

    fwrite(string, 1, strlen(string), fp);
    fwrite("\n", 1,1, fp);

    fclose(fp);
}
