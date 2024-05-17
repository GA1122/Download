BufCompressedClose (BufFilePtr f, int doClose)
{
    CompressedFile  *file;
    BufFilePtr	    raw;

    file = (CompressedFile *) f->private;
    raw = file->file;
    free (file);
    BufFileClose (raw, doClose);
    return 1;
}
