png_set_crc_action(png_structp png_ptr, int crit_action, int ancil_action)
{
   png_debug(1, "in png_set_crc_action");
 
   if (png_ptr == NULL)
      return;

    
   switch (crit_action)
   {
      case PNG_CRC_NO_CHANGE:                         
         break;

      case PNG_CRC_WARN_USE:                                
         png_ptr->flags &= ~PNG_FLAG_CRC_CRITICAL_MASK;
         png_ptr->flags |= PNG_FLAG_CRC_CRITICAL_USE;
         break;

      case PNG_CRC_QUIET_USE:                              
         png_ptr->flags &= ~PNG_FLAG_CRC_CRITICAL_MASK;
         png_ptr->flags |= PNG_FLAG_CRC_CRITICAL_USE |
                           PNG_FLAG_CRC_CRITICAL_IGNORE;
         break;

      case PNG_CRC_WARN_DISCARD:     
         png_warning(png_ptr,
            "Can't discard critical data on CRC error.");
      case PNG_CRC_ERROR_QUIT:                                 

      case PNG_CRC_DEFAULT:
      default:
         png_ptr->flags &= ~PNG_FLAG_CRC_CRITICAL_MASK;
         break;
   }

    
   switch (ancil_action)
   {
      case PNG_CRC_NO_CHANGE:                        
         break;

      case PNG_CRC_WARN_USE:                               
         png_ptr->flags &= ~PNG_FLAG_CRC_ANCILLARY_MASK;
         png_ptr->flags |= PNG_FLAG_CRC_ANCILLARY_USE;
         break;

      case PNG_CRC_QUIET_USE:                             
         png_ptr->flags &= ~PNG_FLAG_CRC_ANCILLARY_MASK;
         png_ptr->flags |= PNG_FLAG_CRC_ANCILLARY_USE |
                           PNG_FLAG_CRC_ANCILLARY_NOWARN;
         break;

      case PNG_CRC_ERROR_QUIT:                                
         png_ptr->flags &= ~PNG_FLAG_CRC_ANCILLARY_MASK;
         png_ptr->flags |= PNG_FLAG_CRC_ANCILLARY_NOWARN;
         break;

      case PNG_CRC_WARN_DISCARD:                       

      case PNG_CRC_DEFAULT:
      default:
         png_ptr->flags &= ~PNG_FLAG_CRC_ANCILLARY_MASK;
         break;
   }
}
