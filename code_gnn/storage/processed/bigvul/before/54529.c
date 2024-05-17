static int mov_read_mdat(MOVContext *c, AVIOContext *pb, MOVAtom atom)
{
    if (atom.size == 0)  
        return 0;
    c->found_mdat=1;
    return 0;  
}
