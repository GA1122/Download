static int mov_read_dops(MOVContext *c, AVIOContext *pb, MOVAtom atom)
{
    const int OPUS_SEEK_PREROLL_MS = 80;
    AVStream *st;
    size_t size;
    int16_t pre_skip;

    if (c->fc->nb_streams < 1)
        return 0;
    st = c->fc->streams[c->fc->nb_streams-1];

    if ((uint64_t)atom.size > (1<<30) || atom.size < 11)
        return AVERROR_INVALIDDATA;

     
    if (avio_r8(pb) != 0) {
        av_log(c->fc, AV_LOG_ERROR, "unsupported OpusSpecificBox version\n");
        return AVERROR_INVALIDDATA;
    }

     
    size = atom.size + 8;

    if (ff_alloc_extradata(st->codecpar, size))
        return AVERROR(ENOMEM);

    AV_WL32(st->codecpar->extradata, MKTAG('O','p','u','s'));
    AV_WL32(st->codecpar->extradata + 4, MKTAG('H','e','a','d'));
    AV_WB8(st->codecpar->extradata + 8, 1);  
    avio_read(pb, st->codecpar->extradata + 9, size - 9);

     
    pre_skip = AV_RB16(st->codecpar->extradata + 10);
    AV_WL16(st->codecpar->extradata + 10, pre_skip);
    AV_WL32(st->codecpar->extradata + 12, AV_RB32(st->codecpar->extradata + 12));
    AV_WL16(st->codecpar->extradata + 16, AV_RB16(st->codecpar->extradata + 16));

    st->codecpar->initial_padding = pre_skip;
    st->codecpar->seek_preroll = av_rescale_q(OPUS_SEEK_PREROLL_MS,
                                              (AVRational){1, 1000},
                                              (AVRational){1, 48000});

    return 0;
}
