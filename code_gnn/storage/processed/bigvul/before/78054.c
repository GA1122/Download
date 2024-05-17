void WriteStr(SAVESTREAM* f, const char *str)
{
    cmsUInt32Number len;

    if (str == NULL)
        str = " ";

    len = (cmsUInt32Number) strlen(str);
    f ->Used += len;


    if (f ->stream) {    

        if (fwrite(str, 1, len, f->stream) != len) {
            cmsSignalError(0, cmsERROR_WRITE, "Write to file error in CGATS parser");
            return;
        }

    }
    else {   

        if (f ->Base) {    

            if (f ->Used > f ->Max) {

                 cmsSignalError(0, cmsERROR_WRITE, "Write to memory overflows in CGATS parser");
                 return;
            }

            memmove(f ->Ptr, str, len);
            f->Ptr += len;
        }

    }
}
