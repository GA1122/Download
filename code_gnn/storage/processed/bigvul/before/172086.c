static inline void byte2char(const char* data, char** str)
{
 **str = *data < ' ' ? '.' : *data > '~' ? '.' : *data;
 ++(*str);
}
