arcStrokeCircularString(wkbObj *w, double segment_angle, lineObj *line)
{
  pointObj p1, p2, p3;
  int npoints, nedges;
  int edge = 0;
  pointArrayObj *pa;

  if ( ! w || ! line ) return MS_FAILURE;

  npoints = wkbReadInt(w);
  nedges = npoints / 2;

   
  if ( npoints < 3 || npoints % 2 != 1 )
    return MS_FAILURE;

   
  pa = pointArrayNew(nedges * 180 / segment_angle);

  wkbReadPointP(w,&p3);

   
  while( edge < nedges ) {
    p1 = p3;
    wkbReadPointP(w,&p2);
    wkbReadPointP(w,&p3);
    if ( arcStrokeCircle(&p1, &p2, &p3, segment_angle, edge ? 0 : 1, pa) == MS_FAILURE ) {
      pointArrayFree(pa);
      return MS_FAILURE;
    }
    edge++;
  }

   
  line->numpoints = pa->npoints;
  line->point = msSmallMalloc(line->numpoints * sizeof(pointObj));
  memcpy(line->point, pa->data, line->numpoints * sizeof(pointObj));

   
  pointArrayFree(pa);

  return MS_SUCCESS;
}
