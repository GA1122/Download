static enum TIFFReadDirEntryErr TIFFReadDirEntrySbyteArray(TIFF* tif, TIFFDirEntry* direntry, int8** value)
{
	enum TIFFReadDirEntryErr err;
	uint32 count;
	void* origdata;
	int8* data;
	switch (direntry->tdir_type)
	{
		case TIFF_UNDEFINED:
		case TIFF_BYTE:
		case TIFF_SBYTE:
		case TIFF_SHORT:
		case TIFF_SSHORT:
		case TIFF_LONG:
		case TIFF_SLONG:
		case TIFF_LONG8:
		case TIFF_SLONG8:
			break;
		default:
			return(TIFFReadDirEntryErrType);
	}
	err=TIFFReadDirEntryArray(tif,direntry,&count,1,&origdata);
	if ((err!=TIFFReadDirEntryErrOk)||(origdata==0))
	{
		*value=0;
		return(err);
	}
	switch (direntry->tdir_type)
	{
		case TIFF_UNDEFINED:
		case TIFF_BYTE:
			{
				uint8* m;
				uint32 n;
				m=(uint8*)origdata;
				for (n=0; n<count; n++)
				{
					err=TIFFReadDirEntryCheckRangeSbyteByte(*m);
					if (err!=TIFFReadDirEntryErrOk)
					{
						_TIFFfree(origdata);
						return(err);
					}
					m++;
				}
				*value=(int8*)origdata;
				return(TIFFReadDirEntryErrOk);
			}
		case TIFF_SBYTE:
			*value=(int8*)origdata;
			return(TIFFReadDirEntryErrOk);
	}
	data=(int8*)_TIFFmalloc(count);
	if (data==0)
	{
		_TIFFfree(origdata);
		return(TIFFReadDirEntryErrAlloc);
	}
	switch (direntry->tdir_type)
	{
		case TIFF_SHORT:
			{
				uint16* ma;
				int8* mb;
				uint32 n;
				ma=(uint16*)origdata;
				mb=data;
				for (n=0; n<count; n++)
				{
					if (tif->tif_flags&TIFF_SWAB)
						TIFFSwabShort(ma);
					err=TIFFReadDirEntryCheckRangeSbyteShort(*ma);
					if (err!=TIFFReadDirEntryErrOk)
						break;
					*mb++=(int8)(*ma++);
				}
			}
			break;
		case TIFF_SSHORT:
			{
				int16* ma;
				int8* mb;
				uint32 n;
				ma=(int16*)origdata;
				mb=data;
				for (n=0; n<count; n++)
				{
					if (tif->tif_flags&TIFF_SWAB)
						TIFFSwabShort((uint16*)ma);
					err=TIFFReadDirEntryCheckRangeSbyteSshort(*ma);
					if (err!=TIFFReadDirEntryErrOk)
						break;
					*mb++=(int8)(*ma++);
				}
			}
			break;
		case TIFF_LONG:
			{
				uint32* ma;
				int8* mb;
				uint32 n;
				ma=(uint32*)origdata;
				mb=data;
				for (n=0; n<count; n++)
				{
					if (tif->tif_flags&TIFF_SWAB)
						TIFFSwabLong(ma);
					err=TIFFReadDirEntryCheckRangeSbyteLong(*ma);
					if (err!=TIFFReadDirEntryErrOk)
						break;
					*mb++=(int8)(*ma++);
				}
			}
			break;
		case TIFF_SLONG:
			{
				int32* ma;
				int8* mb;
				uint32 n;
				ma=(int32*)origdata;
				mb=data;
				for (n=0; n<count; n++)
				{
					if (tif->tif_flags&TIFF_SWAB)
						TIFFSwabLong((uint32*)ma);
					err=TIFFReadDirEntryCheckRangeSbyteSlong(*ma);
					if (err!=TIFFReadDirEntryErrOk)
						break;
					*mb++=(int8)(*ma++);
				}
			}
			break;
		case TIFF_LONG8:
			{
				uint64* ma;
				int8* mb;
				uint32 n;
				ma=(uint64*)origdata;
				mb=data;
				for (n=0; n<count; n++)
				{
					if (tif->tif_flags&TIFF_SWAB)
						TIFFSwabLong8(ma);
					err=TIFFReadDirEntryCheckRangeSbyteLong8(*ma);
					if (err!=TIFFReadDirEntryErrOk)
						break;
					*mb++=(int8)(*ma++);
				}
			}
			break;
		case TIFF_SLONG8:
			{
				int64* ma;
				int8* mb;
				uint32 n;
				ma=(int64*)origdata;
				mb=data;
				for (n=0; n<count; n++)
				{
					if (tif->tif_flags&TIFF_SWAB)
						TIFFSwabLong8((uint64*)ma);
					err=TIFFReadDirEntryCheckRangeSbyteSlong8(*ma);
					if (err!=TIFFReadDirEntryErrOk)
						break;
					*mb++=(int8)(*ma++);
				}
			}
			break;
	}
	_TIFFfree(origdata);
	if (err!=TIFFReadDirEntryErrOk)
	{
		_TIFFfree(data);
		return(err);
	}
	*value=data;
	return(TIFFReadDirEntryErrOk);
}