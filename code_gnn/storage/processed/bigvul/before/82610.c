bool isWhitespace(char ch) {
    return (ch==0x09) ||  
           (ch==0x0B) ||  
           (ch==0x0C) ||  
           (ch==0x20) ||  
           (ch=='\n') ||
           (ch=='\r');
}
