int CALLBACK EnhMetaFileProc(HDC dc,
                             HANDLETABLE* handle_table,
                             const ENHMETARECORD *record,
                             int num_objects,
                             LPARAM data) {
  HDC* bitmap_dc = reinterpret_cast<HDC*>(data);
  PlayEnhMetaFileRecord(*bitmap_dc, handle_table, record, num_objects);
  if (record->iType == EMR_ALPHABLEND) {
    const EMRALPHABLEND* emr_alpha_blend =
        reinterpret_cast<const EMRALPHABLEND*>(record);
    XFORM bitmap_dc_transform, metafile_dc_transform;
    XFORM identity = { 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f };
    GetWorldTransform(dc, &metafile_dc_transform);
    SetWorldTransform(dc, &identity);
    GetWorldTransform(*bitmap_dc, &bitmap_dc_transform);
    SetWorldTransform(*bitmap_dc, &identity);
    const RECTL& rect = emr_alpha_blend->rclBounds;
    BitBlt(dc,
           rect.left,
           rect.top,
           rect.right - rect.left + 1,
           rect.bottom - rect.top + 1,
           *bitmap_dc,
           rect.left,
           rect.top,
           SRCCOPY);
    SetWorldTransform(dc, &metafile_dc_transform);
    SetWorldTransform(*bitmap_dc, &bitmap_dc_transform);
  } else {
    PlayEnhMetaFileRecord(dc, handle_table, record, num_objects);
  }
  return 1;   
}
