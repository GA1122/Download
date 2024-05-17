static int is_cover_image(const AVStream *st)
{
     
    return st && st->disposition == AV_DISPOSITION_ATTACHED_PIC;
}
