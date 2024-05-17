void read_mv(stream_t *stream,mv_t *mv,mv_t *mvp)
{
    mv_t mvd;
    int mvabs, mvsign = 0;

     
    if ((mvabs = get_vlc(7, stream)))
      mvsign = get_flc(1, stream);
    mvd.x = mvabs * (mvsign ? -1 : 1);
    mv->x = mvp->x + mvd.x;

     
    if ((mvabs = get_vlc(7, stream)))
      mvsign = get_flc(1, stream);
    mvd.y = mvabs * (mvsign ? -1 : 1);
    mv->y = mvp->y + mvd.y;
}