void dump_bin(const char* title, const char* data, int size)
{
 char line_buff[256];
 char *line;
 int i, j, addr;
 const int width = 16;
    LOG_DEBUG("%s, size:%d, dump started {", title, size);
 if(size <= 0)
 return;
    line = line_buff;
 *line++ = ' ';
 *line++ = ' ';
 *line++ = ' ';
 *line++ = ' ';
 *line++ = ' ';
 *line++ = ' ';
 for(j = 0; j < width; j++)
 {
        byte2hex((const char*)&j, &line);
 *line++ = ' ';
 }
 *line = 0;
    PRINT(line_buff);

 for(i = 0; i < size / width; i++)
 {
        line = line_buff;
        addr = i*width;
        word2hex((const char*)&addr, &line);
 *line++ = ':'; *line++ = ' ';
 for(j = 0; j < width; j++)
 {
            byte2hex(&data[j], &line);
 *line++ = ' ';
 }
 for(j = 0; j < width; j++)
            byte2char(data++, &line);
 *line = 0;
        PRINT(line_buff);
 }
 int leftover = size % width;
 if(leftover > 0)
 {
        line = line_buff;
        addr = i*width;
        word2hex((const char*)&addr, &line);
 *line++ = ':'; *line++ = ' ';
 for(j = 0; j < leftover; j++) {
            byte2hex(&data[j], &line);
 *line++ = ' ';
 }
 for(; j < width; j++) {
 *line++ = ' ';
 *line++ = ' ';
 *line++ = ' ';
 }
 for(j = 0; j < leftover; j++)
            byte2char(data++, &line);
 *line = 0;
        PRINT(line_buff);
 }
    LOG_DEBUG("%s, size:%d, dump ended }", title, size);
}
