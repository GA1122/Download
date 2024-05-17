local void grow_space(struct space *space)
{
    size_t more;

     
    more = grow(space->size);
    if (more == space->size)
        bail("not enough memory", "");

     
    space->buf = REALLOC(space->buf, more);
    if (space->buf == NULL)
        bail("not enough memory", "");
    space->size = more;
}
