static SegmentInfo AffineEdge(const Image *image,const AffineMatrix *affine,
  const double y,const SegmentInfo *edge)
{
  double
    intercept,
    z;

  register double
    x;

  SegmentInfo
    inverse_edge;

   
  inverse_edge.x1=edge->x1;
  inverse_edge.y1=edge->y1;
  inverse_edge.x2=edge->x2;
  inverse_edge.y2=edge->y2;
  z=affine->ry*y+affine->tx;
  if (affine->sx >= DrawEpsilon)
    {
      intercept=(-z/affine->sx);
      x=intercept;
      if (x > inverse_edge.x1)
        inverse_edge.x1=x;
      intercept=(-z+(double) image->columns)/affine->sx;
      x=intercept;
      if (x < inverse_edge.x2)
        inverse_edge.x2=x;
    }
  else
    if (affine->sx < -DrawEpsilon)
      {
        intercept=(-z+(double) image->columns)/affine->sx;
        x=intercept;
        if (x > inverse_edge.x1)
          inverse_edge.x1=x;
        intercept=(-z/affine->sx);
        x=intercept;
        if (x < inverse_edge.x2)
          inverse_edge.x2=x;
      }
    else
      if ((z < 0.0) || ((size_t) floor(z+0.5) >= image->columns))
        {
          inverse_edge.x2=edge->x1;
          return(inverse_edge);
        }
   
  z=affine->sy*y+affine->ty;
  if (affine->rx >= DrawEpsilon)
    {
      intercept=(-z/affine->rx);
      x=intercept;
      if (x > inverse_edge.x1)
        inverse_edge.x1=x;
      intercept=(-z+(double) image->rows)/affine->rx;
      x=intercept;
      if (x < inverse_edge.x2)
        inverse_edge.x2=x;
    }
  else
    if (affine->rx < -DrawEpsilon)
      {
        intercept=(-z+(double) image->rows)/affine->rx;
        x=intercept;
        if (x > inverse_edge.x1)
          inverse_edge.x1=x;
        intercept=(-z/affine->rx);
        x=intercept;
        if (x < inverse_edge.x2)
          inverse_edge.x2=x;
      }
    else
      if ((z < 0.0) || ((size_t) floor(z+0.5) >= image->rows))
        {
          inverse_edge.x2=edge->x2;
          return(inverse_edge);
        }
  return(inverse_edge);
}