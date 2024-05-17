void send_error(int error)
{
    const char *the_error;

    switch (error) {

    case 1:
        the_error = "Not enough arguments were passed to the indexer.";
        break;
    case 2:
        the_error = "The Directory Sorter ran out of Memory";
        break;
    case 3:
        the_error =
            "The was a problem changing to the appropriate directory.";
        break;
    case 4:
        the_error = "There was an error escaping a string.";
    case 5:
        the_error = "Too many arguments were passed to the indexer.";
        break;
    case 6:
        the_error = "No files in this directory.";
        break;
    default:
        the_error = "An unknown error occurred producing the directory.";
        break;
    }
    printf("<html>\n<head>\n<title>\n%s\n</title>\n"
           "<body>\n%s\n</body>\n</html>\n", the_error, the_error);
}
