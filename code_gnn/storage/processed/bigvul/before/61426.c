static int mov_read_default(MOVContext *c, AVIOContext *pb, MOVAtom atom)
{
    int64_t total_size = 0;
    MOVAtom a;
    int i;

    if (c->atom_depth > 10) {
        av_log(c->fc, AV_LOG_ERROR, "Atoms too deeply nested\n");
        return AVERROR_INVALIDDATA;
    }
    c->atom_depth ++;

    if (atom.size < 0)
        atom.size = INT64_MAX;
    while (total_size <= atom.size - 8 && !avio_feof(pb)) {
        int (*parse)(MOVContext*, AVIOContext*, MOVAtom) = NULL;
        a.size = atom.size;
        a.type=0;
        if (atom.size >= 8) {
            a.size = avio_rb32(pb);
            a.type = avio_rl32(pb);
            if (a.type == MKTAG('f','r','e','e') &&
                a.size >= 8 &&
                c->fc->strict_std_compliance < FF_COMPLIANCE_STRICT &&
                c->moov_retry) {
                uint8_t buf[8];
                uint32_t *type = (uint32_t *)buf + 1;
                if (avio_read(pb, buf, 8) != 8)
                    return AVERROR_INVALIDDATA;
                avio_seek(pb, -8, SEEK_CUR);
                if (*type == MKTAG('m','v','h','d') ||
                    *type == MKTAG('c','m','o','v')) {
                    av_log(c->fc, AV_LOG_ERROR, "Detected moov in a free atom.\n");
                    a.type = MKTAG('m','o','o','v');
                }
            }
            if (atom.type != MKTAG('r','o','o','t') &&
                atom.type != MKTAG('m','o','o','v'))
            {
                if (a.type == MKTAG('t','r','a','k') || a.type == MKTAG('m','d','a','t'))
                {
                    av_log(c->fc, AV_LOG_ERROR, "Broken file, trak/mdat not at top-level\n");
                    avio_skip(pb, -8);
                    c->atom_depth --;
                    return 0;
                }
            }
            total_size += 8;
            if (a.size == 1 && total_size + 8 <= atom.size) {  
                a.size = avio_rb64(pb) - 8;
                total_size += 8;
            }
        }
        av_log(c->fc, AV_LOG_TRACE, "type:'%s' parent:'%s' sz: %"PRId64" %"PRId64" %"PRId64"\n",
               av_fourcc2str(a.type), av_fourcc2str(atom.type), a.size, total_size, atom.size);
        if (a.size == 0) {
            a.size = atom.size - total_size + 8;
        }
        a.size -= 8;
        if (a.size < 0)
            break;
        a.size = FFMIN(a.size, atom.size - total_size);

        for (i = 0; mov_default_parse_table[i].type; i++)
            if (mov_default_parse_table[i].type == a.type) {
                parse = mov_default_parse_table[i].parse;
                break;
            }

        if (!parse && (atom.type == MKTAG('u','d','t','a') ||
                       atom.type == MKTAG('i','l','s','t')))
            parse = mov_read_udta_string;

        if (!parse && c->found_hdlr_mdta &&
            atom.type == MKTAG('m','e','t','a') &&
            a.type == MKTAG('k','e','y','s')) {
            parse = mov_read_keys;
        }

        if (!parse) {  
            avio_skip(pb, a.size);
        } else {
            int64_t start_pos = avio_tell(pb);
            int64_t left;
            int err = parse(c, pb, a);
            if (err < 0) {
                c->atom_depth --;
                return err;
            }
            if (c->found_moov && c->found_mdat &&
                ((!(pb->seekable & AVIO_SEEKABLE_NORMAL) || c->fc->flags & AVFMT_FLAG_IGNIDX || c->fragment_index_complete) ||
                 start_pos + a.size == avio_size(pb))) {
                if (!(pb->seekable & AVIO_SEEKABLE_NORMAL) || c->fc->flags & AVFMT_FLAG_IGNIDX || c->fragment_index_complete)
                    c->next_root_atom = start_pos + a.size;
                c->atom_depth --;
                return 0;
            }
            left = a.size - avio_tell(pb) + start_pos;
            if (left > 0)  
                avio_skip(pb, left);
            else if (left < 0) {
                av_log(c->fc, AV_LOG_WARNING,
                       "overread end of atom '%.4s' by %"PRId64" bytes\n",
                       (char*)&a.type, -left);
                avio_seek(pb, left, SEEK_CUR);
            }
        }

        total_size += a.size;
    }

    if (total_size < atom.size && atom.size < 0x7ffff)
        avio_skip(pb, atom.size - total_size);

    c->atom_depth --;
    return 0;
}