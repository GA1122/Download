static inline char* getEmptyString()
{
    gEmptyStringBuf->acquire();
 return gEmptyString;
}
