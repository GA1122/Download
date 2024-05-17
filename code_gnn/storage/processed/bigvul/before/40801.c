arcStrokeCircle(const pointObj *p1, const pointObj *p2, const pointObj *p3,
                double segment_angle, int include_first, pointArrayObj *pa)
{
  pointObj center;  
  double radius;  
  double sweep_angle_r;  
  double segment_angle_r;  
  double a1,   a3;  
  int side = arcSegmentSide(p1, p3, p2);  
  int num_edges;  
  double current_angle_r;  
  int i;  
  pointObj p;  
  int is_closed = MS_FALSE;

   
  if ( FP_EQ(p1->x, p3->x) && FP_EQ(p1->y, p3->y) )
    is_closed = MS_TRUE;

   
  if ( ! is_closed && side == FP_COLINEAR ) {
     
    if ( include_first )
      pointArrayAddPoint(pa, p1);
    pointArrayAddPoint(pa, p3);
    return MS_SUCCESS;
  }

   
  if ( arcCircleCenter(p1, p2, p3, &center, &radius) == MS_FAILURE )
    return MS_FAILURE;

   
  a1 = atan2(p1->y - center.y, p1->x - center.x);
   
  a3 = atan2(p3->y - center.y, p3->x - center.x);
  segment_angle_r = M_PI * segment_angle / 180.0;

   
  if ( is_closed ) {
    sweep_angle_r = 2.0 * M_PI;
  }
   
  else if ( side == FP_LEFT ) {
    if ( a3 > a1 )  
      sweep_angle_r = a1 + (2.0 * M_PI - a3);
    else
      sweep_angle_r = a1 - a3;
  }
   
  else if ( side == FP_RIGHT ) {
    if ( a3 > a1 )  
      sweep_angle_r = a3 - a1;
    else
      sweep_angle_r = a3 + (2.0 * M_PI - a1);
  } else
    sweep_angle_r = 0.0;

   
  if ( (sweep_angle_r / segment_angle_r) < SEGMENT_MINPOINTS ) {
    segment_angle_r = sweep_angle_r / (SEGMENT_MINPOINTS + 1);
  }

   
  if ( sweep_angle_r < segment_angle_r ) {
    if ( include_first )
      pointArrayAddPoint(pa, p1);
    pointArrayAddPoint(pa, p3);
    return MS_SUCCESS;
  }

   
  num_edges = floor(sweep_angle_r / fabs(segment_angle_r));

   
  if ( side == FP_LEFT )
    segment_angle_r *= -1;

   
  if( include_first ) {
    current_angle_r = a1;
  } else {
    current_angle_r = a1 + segment_angle_r;
    num_edges--;
  }

   
  for( i = 0; i < num_edges; i++ ) {
    if (segment_angle_r > 0.0 && current_angle_r > M_PI)
      current_angle_r -= 2*M_PI;
    if (segment_angle_r < 0.0 && current_angle_r < -1*M_PI)
      current_angle_r -= 2*M_PI;
    p.x = center.x + radius*cos(current_angle_r);
    p.y = center.y + radius*sin(current_angle_r);
    pointArrayAddPoint(pa, &p);
    current_angle_r += segment_angle_r;
  }

   
  pointArrayAddPoint(pa, p3);
  return MS_SUCCESS;
}
