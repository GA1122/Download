static String makeURLString(const char* url)
{
    String urlString(url);
    
    urlString.replace('\r', "");
    urlString.replace('\n', "");

    return urlString;
}
