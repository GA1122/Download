pango_glyph_string_x_to_index (PangoGlyphString *glyphs,
			       char             *text,
			       int               length,
			       PangoAnalysis    *analysis,
			       int               x_pos,
			       int              *index,
			       gboolean         *trailing)
{
  int i;
  int start_xpos = 0;
  int end_xpos = 0;
  int width = 0;

  int start_index = -1;
  int end_index = -1;

  int cluster_chars = 0;
  char *p;

  gboolean found = FALSE;

   

  width = 0;

  if (analysis->level % 2)  
    {
      for (i = glyphs->num_glyphs - 1; i >= 0; i--)
	width += glyphs->glyphs[i].geometry.width;

      for (i = glyphs->num_glyphs - 1; i >= 0; i--)
	{
	  if (glyphs->log_clusters[i] != start_index)
	    {
	      if (found)
		{
		  end_index = glyphs->log_clusters[i];
		  end_xpos = width;
		  break;
		}
	      else
		{
		  start_index = glyphs->log_clusters[i];
		  start_xpos = width;
		}
	    }

	  width -= glyphs->glyphs[i].geometry.width;

	  if (width <= x_pos && x_pos < width + glyphs->glyphs[i].geometry.width)
	    found = TRUE;
	}
    }
  else  
    {
      for (i = 0; i < glyphs->num_glyphs; i++)
	{
	  if (glyphs->log_clusters[i] != start_index)
	    {
	      if (found)
		{
		  end_index = glyphs->log_clusters[i];
		  end_xpos = width;
		  break;
		}
	      else
		{
		  start_index = glyphs->log_clusters[i];
		  start_xpos = width;
		}
	    }

	  if (width <= x_pos && x_pos < width + glyphs->glyphs[i].geometry.width)
	    found = TRUE;

	  width += glyphs->glyphs[i].geometry.width;
	}
    }

  if (end_index == -1)
    {
      end_index = length;
      end_xpos = (analysis->level % 2) ? 0 : width;
    }

   
  p = text + start_index;
  while (p < text + end_index)
    {
      p = g_utf8_next_char (p);
      cluster_chars++;
    }

  if (start_xpos == end_xpos)
    {
      if (index)
	*index = start_index;
      if (trailing)
	*trailing = FALSE;
    }
  else
    {
      double cp = ((double)(x_pos - start_xpos) * cluster_chars) / (end_xpos - start_xpos);

       
      if (start_xpos < end_xpos)  
	{
	  if (index)
	    {
	      char *p = text + start_index;
	      int i = 0;

	      while (i + 1 <= cp)
		{
		  p = g_utf8_next_char (p);
		  i++;
		}

	      *index = (p - text);
	    }

	  if (trailing)
	    *trailing = (cp - (int)cp >= 0.5) ? TRUE : FALSE;
	}
      else  
	{
	  if (index)
	    {
	      char *p = text + start_index;
	      int i = 0;

	      while (i + 1 < cp)
		{
		  p = g_utf8_next_char (p);
		  i++;
		}

	      *index = (p - text);
	    }

	  if (trailing)
	    {
	      double cp_flip = cluster_chars - cp;
	      *trailing = (cp_flip - (int)cp_flip >= 0.5) ? FALSE : TRUE;
	    }
	}
    }
}
