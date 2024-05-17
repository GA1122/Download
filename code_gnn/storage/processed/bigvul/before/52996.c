static PolygonInfo *ConvertPathToPolygon(const PathInfo *path_info)
{
  long
    direction,
    next_direction;

  PointInfo
    point,
    *points;

  PolygonInfo
    *polygon_info;

  SegmentInfo
    bounds;

  register ssize_t
    i,
    n;

  MagickBooleanType
    ghostline;

  size_t
    edge,
    number_edges,
    number_points;

   
  polygon_info=(PolygonInfo *) AcquireMagickMemory(sizeof(*polygon_info));
  if (polygon_info == (PolygonInfo *) NULL)
    return((PolygonInfo *) NULL);
  number_edges=16;
  polygon_info->edges=(EdgeInfo *) AcquireQuantumMemory((size_t) number_edges,
    sizeof(*polygon_info->edges));
  if (polygon_info->edges == (EdgeInfo *) NULL)
    return((PolygonInfo *) NULL);
  direction=0;
  edge=0;
  ghostline=MagickFalse;
  n=0;
  number_points=0;
  points=(PointInfo *) NULL;
  (void) ResetMagickMemory(&point,0,sizeof(point));
  (void) ResetMagickMemory(&bounds,0,sizeof(bounds));
  for (i=0; path_info[i].code != EndCode; i++)
  {
    if ((path_info[i].code == MoveToCode) || (path_info[i].code == OpenCode) ||
        (path_info[i].code == GhostlineCode))
      {
         
        if ((points != (PointInfo *) NULL) && (n >= 2))
          {
            if (edge == number_edges)
              {
                number_edges<<=1;
                polygon_info->edges=(EdgeInfo *) ResizeQuantumMemory(
                  polygon_info->edges,(size_t) number_edges,
                  sizeof(*polygon_info->edges));
                if (polygon_info->edges == (EdgeInfo *) NULL)
                  return((PolygonInfo *) NULL);
              }
            polygon_info->edges[edge].number_points=(size_t) n;
            polygon_info->edges[edge].scanline=(-1.0);
            polygon_info->edges[edge].highwater=0;
            polygon_info->edges[edge].ghostline=ghostline;
            polygon_info->edges[edge].direction=(ssize_t) (direction > 0);
            if (direction < 0)
              ReversePoints(points,(size_t) n);
            polygon_info->edges[edge].points=points;
            polygon_info->edges[edge].bounds=bounds;
            polygon_info->edges[edge].bounds.y1=points[0].y;
            polygon_info->edges[edge].bounds.y2=points[n-1].y;
            points=(PointInfo *) NULL;
            ghostline=MagickFalse;
            edge++;
          }
        if (points == (PointInfo *) NULL)
          {
            number_points=16;
            points=(PointInfo *) AcquireQuantumMemory((size_t) number_points,
              sizeof(*points));
            if (points == (PointInfo *) NULL)
              return((PolygonInfo *) NULL);
          }
        ghostline=path_info[i].code == GhostlineCode ? MagickTrue : MagickFalse;
        point=path_info[i].point;
        points[0]=point;
        bounds.x1=point.x;
        bounds.x2=point.x;
        direction=0;
        n=1;
        continue;
      }
     
    next_direction=((path_info[i].point.y > point.y) ||
      ((path_info[i].point.y == point.y) &&
       (path_info[i].point.x > point.x))) ? 1 : -1;
    if ((points != (PointInfo *) NULL) && (direction != 0) &&
        (direction != next_direction))
      {
         
        point=points[n-1];
        if (edge == number_edges)
          {
            number_edges<<=1;
            polygon_info->edges=(EdgeInfo *) ResizeQuantumMemory(
              polygon_info->edges,(size_t) number_edges,
              sizeof(*polygon_info->edges));
            if (polygon_info->edges == (EdgeInfo *) NULL)
              return((PolygonInfo *) NULL);
          }
        polygon_info->edges[edge].number_points=(size_t) n;
        polygon_info->edges[edge].scanline=(-1.0);
        polygon_info->edges[edge].highwater=0;
        polygon_info->edges[edge].ghostline=ghostline;
        polygon_info->edges[edge].direction=(ssize_t) (direction > 0);
        if (direction < 0)
          ReversePoints(points,(size_t) n);
        polygon_info->edges[edge].points=points;
        polygon_info->edges[edge].bounds=bounds;
        polygon_info->edges[edge].bounds.y1=points[0].y;
        polygon_info->edges[edge].bounds.y2=points[n-1].y;
        number_points=16;
        points=(PointInfo *) AcquireQuantumMemory((size_t) number_points,
          sizeof(*points));
        if (points == (PointInfo *) NULL)
          return((PolygonInfo *) NULL);
        n=1;
        ghostline=MagickFalse;
        points[0]=point;
        bounds.x1=point.x;
        bounds.x2=point.x;
        edge++;
      }
    direction=next_direction;
    if (points == (PointInfo *) NULL)
      continue;
    if (n == (ssize_t) number_points)
      {
        number_points<<=1;
        points=(PointInfo *) ResizeQuantumMemory(points,(size_t) number_points,
          sizeof(*points));
        if (points == (PointInfo *) NULL)
          return((PolygonInfo *) NULL);
      }
    point=path_info[i].point;
    points[n]=point;
    if (point.x < bounds.x1)
      bounds.x1=point.x;
    if (point.x > bounds.x2)
      bounds.x2=point.x;
    n++;
  }
  if (points != (PointInfo *) NULL)
    {
      if (n < 2)
        points=(PointInfo *) RelinquishMagickMemory(points);
      else
        {
          if (edge == number_edges)
            {
              number_edges<<=1;
              polygon_info->edges=(EdgeInfo *) ResizeQuantumMemory(
                polygon_info->edges,(size_t) number_edges,
                sizeof(*polygon_info->edges));
              if (polygon_info->edges == (EdgeInfo *) NULL)
                return((PolygonInfo *) NULL);
            }
          polygon_info->edges[edge].number_points=(size_t) n;
          polygon_info->edges[edge].scanline=(-1.0);
          polygon_info->edges[edge].highwater=0;
          polygon_info->edges[edge].ghostline=ghostline;
          polygon_info->edges[edge].direction=(ssize_t) (direction > 0);
          if (direction < 0)
            ReversePoints(points,(size_t) n);
          polygon_info->edges[edge].points=points;
          polygon_info->edges[edge].bounds=bounds;
          polygon_info->edges[edge].bounds.y1=points[0].y;
          polygon_info->edges[edge].bounds.y2=points[n-1].y;
          ghostline=MagickFalse;
          edge++;
        }
    }
  polygon_info->number_edges=edge;
  qsort(polygon_info->edges,(size_t) polygon_info->number_edges,
    sizeof(*polygon_info->edges),CompareEdges);
  if (IsEventLogging() != MagickFalse)
    LogPolygonInfo(polygon_info);
  return(polygon_info);
}