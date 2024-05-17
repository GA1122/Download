static int getFileStartAndLength(int fd, off_t *start_, size_t *length_)
{
 off_t start, end;
 size_t length;

    assert(start_ != NULL);
    assert(length_ != NULL);

    start = lseek(fd, 0L, SEEK_CUR);
    end = lseek(fd, 0L, SEEK_END);
 (void) lseek(fd, start, SEEK_SET);

 if (start == (off_t) -1 || end == (off_t) -1) {
        LOGE("could not determine length of file\n");
 return -1;
 }

    length = end - start;
 if (length == 0) {
        LOGE("file is empty\n");
 return -1;
 }

 *start_ = start;
 *length_ = length;

 return 0;
}
