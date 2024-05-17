static void fbFetchSourcePict(PicturePtr pict, int x, int y, int width, CARD32 *buffer, CARD32 *mask, CARD32 maskBits)
{
     SourcePictPtr   pGradient = pict->pSourcePict;
     GradientWalker  walker;
     CARD32         *end = buffer + width;
 
     _gradient_walker_init (&walker, pGradient, pict->repeat);
    
    if (pGradient->type == SourcePictTypeSolidFill) {
        register CARD32 color = pGradient->solidFill.color;
        while (buffer < end) {
            WRITE(buffer++, color);
        }
    } else if (pGradient->type == SourcePictTypeLinear) {
        PictVector v, unit;
        xFixed_32_32 l;
        xFixed_48_16 dx, dy, a, b, off;
	
         
        v.vector[0] = IntToxFixed(x) + xFixed1/2;
        v.vector[1] = IntToxFixed(y) + xFixed1/2;
        v.vector[2] = xFixed1;
        if (pict->transform) {
            if (!PictureTransformPoint3d (pict->transform, &v))
                return;
            unit.vector[0] = pict->transform->matrix[0][0];
            unit.vector[1] = pict->transform->matrix[1][0];
            unit.vector[2] = pict->transform->matrix[2][0];
        } else {
            unit.vector[0] = xFixed1;
            unit.vector[1] = 0;
            unit.vector[2] = 0;
        }

        dx = pGradient->linear.p2.x - pGradient->linear.p1.x;
        dy = pGradient->linear.p2.y - pGradient->linear.p1.y;
        l = dx*dx + dy*dy;
        if (l != 0) {
            a = (dx << 32) / l;
            b = (dy << 32) / l;
            off = (-a*pGradient->linear.p1.x - b*pGradient->linear.p1.y)>>16;
        }
        if (l == 0  || (unit.vector[2] == 0 && v.vector[2] == xFixed1)) {
            xFixed_48_16 inc, t;
             
            if (l == 0) {
                t = 0;
                inc = 0;
            } else {
                t = ((a*v.vector[0] + b*v.vector[1]) >> 16) + off;
                inc = (a * unit.vector[0] + b * unit.vector[1]) >> 16;
            }

	    if (pGradient->linear.class == SourcePictClassVertical)
	    {
		register CARD32 color;

		color = _gradient_walker_pixel( &walker, t );
		while (buffer < end)
		    WRITE(buffer++, color);
	    }
	    else
	    {
                if (!mask) {
                    while (buffer < end)
                    {
                        WRITE(buffer, _gradient_walker_pixel (&walker, t));
                        buffer += 1;
                        t      += inc;
                    }
                } else {
                    while (buffer < end) {
                        if (*mask++ & maskBits)
                        {
                            WRITE(buffer, _gradient_walker_pixel (&walker, t));
                        }
                        buffer += 1;
                        t      += inc;
                    }
                }
	    }
	}
	else  
	{
	    xFixed_48_16 t;

	    if (pGradient->linear.class == SourcePictClassVertical)
	    {
		register CARD32 color;

		if (v.vector[2] == 0)
		{
		    t = 0;
		}
		else
		{
		    xFixed_48_16 x, y;

		    x = ((xFixed_48_16) v.vector[0] << 16) / v.vector[2];
		    y = ((xFixed_48_16) v.vector[1] << 16) / v.vector[2];
		    t = ((a * x + b * y) >> 16) + off;
		}

 		color = _gradient_walker_pixel( &walker, t );
		while (buffer < end)
		    WRITE(buffer++, color);
	    }
	    else
	    {
		while (buffer < end)
		{
		    if (!mask || *mask++ & maskBits)
		    {
			if (v.vector[2] == 0) {
			    t = 0;
			} else {
			    xFixed_48_16 x, y;
			    x = ((xFixed_48_16)v.vector[0] << 16) / v.vector[2];
			    y = ((xFixed_48_16)v.vector[1] << 16) / v.vector[2];
			    t = ((a*x + b*y) >> 16) + off;
			}
			WRITE(buffer, _gradient_walker_pixel (&walker, t));
		    }
		    ++buffer;
		    v.vector[0] += unit.vector[0];
		    v.vector[1] += unit.vector[1];
		    v.vector[2] += unit.vector[2];
		}
            }
        }
    } else {

 
         
        Bool affine = TRUE;
        double cx = 1.;
        double cy = 0.;
        double cz = 0.;
	double rx = x + 0.5;
	double ry = y + 0.5;
        double rz = 1.;

        if (pict->transform) {
            PictVector v;
             
            v.vector[0] = IntToxFixed(x) + xFixed1/2;
            v.vector[1] = IntToxFixed(y) + xFixed1/2;
            v.vector[2] = xFixed1;
            if (!PictureTransformPoint3d (pict->transform, &v))
                return;

            cx = pict->transform->matrix[0][0]/65536.;
            cy = pict->transform->matrix[1][0]/65536.;
            cz = pict->transform->matrix[2][0]/65536.;
            rx = v.vector[0]/65536.;
            ry = v.vector[1]/65536.;
            rz = v.vector[2]/65536.;
            affine = pict->transform->matrix[2][0] == 0 && v.vector[2] == xFixed1;
        }

        if (pGradient->type == SourcePictTypeRadial) {
	    PictRadialGradient *radial;
	    radial = &pGradient->radial;
            if (affine) {
                while (buffer < end) {
		    if (!mask || *mask++ & maskBits)
		    {
			double pdx, pdy;
			double B, C;
			double det;
			double c1x = radial->c1.x / 65536.0;
			double c1y = radial->c1.y / 65536.0;
			double r1  = radial->c1.radius / 65536.0;
                        xFixed_48_16 t;

			pdx = rx - c1x;
			pdy = ry - c1y;

			B = -2 * (  pdx * radial->cdx
				  + pdy * radial->cdy
				  + r1 * radial->dr);
			C = (pdx * pdx + pdy * pdy - r1 * r1);

                        det = (B * B) - (4 * radial->A * C);
			if (det < 0.0)
			    det = 0.0;

			if (radial->A < 0)
			    t = (xFixed_48_16) ((- B - sqrt(det)) / (2.0 * radial->A) * 65536);
			else
			    t = (xFixed_48_16) ((- B + sqrt(det)) / (2.0 * radial->A) * 65536);
			
 			WRITE(buffer, _gradient_walker_pixel (&walker, t));
		    }
		    ++buffer;
		    
                    rx += cx;
                    ry += cy;
                }
            } else {
		 
                while (buffer < end) {
		    if (!mask || *mask++ & maskBits)
		    {
			double pdx, pdy;
			double B, C;
			double det;
			double c1x = radial->c1.x / 65536.0;
			double c1y = radial->c1.y / 65536.0;
			double r1  = radial->c1.radius / 65536.0;
                        xFixed_48_16 t;
			double x, y;

			if (rz != 0) {
			    x = rx/rz;
			    y = ry/rz;
			} else {
			    x = y = 0.;
			}
			
			pdx = x - c1x;
			pdy = y - c1y;

			B = -2 * (  pdx * radial->cdx
				  + pdy * radial->cdy
				  + r1 * radial->dr);
			C = (pdx * pdx + pdy * pdy - r1 * r1);

                        det = (B * B) - (4 * radial->A * C);
			if (det < 0.0)
			    det = 0.0;

			if (radial->A < 0)
			    t = (xFixed_48_16) ((- B - sqrt(det)) / (2.0 * radial->A) * 65536);
			else
			    t = (xFixed_48_16) ((- B + sqrt(det)) / (2.0 * radial->A) * 65536);
			
 			WRITE(buffer, _gradient_walker_pixel (&walker, t));
		    }
		    ++buffer;
		    
                    rx += cx;
                    ry += cy;
		    rz += cz;
                }
            }
        } else   {
            double a = pGradient->conical.angle/(180.*65536);
            if (affine) {
                rx -= pGradient->conical.center.x/65536.;
                ry -= pGradient->conical.center.y/65536.;

                while (buffer < end) {
		    double angle;

                    if (!mask || *mask++ & maskBits)
		    {
                        xFixed_48_16   t;
			
                        angle = atan2(ry, rx) + a;
			t     = (xFixed_48_16) (angle * (65536. / (2*M_PI)));
			
			WRITE(buffer, _gradient_walker_pixel (&walker, t));
		    }

                    ++buffer;
                    rx += cx;
                    ry += cy;
                }
            } else {
                while (buffer < end) {
                    double x, y;
                    double angle;
		    
                    if (!mask || *mask++ & maskBits)
                    {
			xFixed_48_16  t;
			
			if (rz != 0) {
			    x = rx/rz;
			    y = ry/rz;
			} else {
			    x = y = 0.;
			}
			x -= pGradient->conical.center.x/65536.;
			y -= pGradient->conical.center.y/65536.;
			angle = atan2(y, x) + a;
			t     = (xFixed_48_16) (angle * (65536. / (2*M_PI)));
			
			WRITE(buffer, _gradient_walker_pixel (&walker, t));
		    }
		    
                    ++buffer;
                    rx += cx;
                    ry += cy;
                    rz += cz;
                }
            }
        }
    }
}
