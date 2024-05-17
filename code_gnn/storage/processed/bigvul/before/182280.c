 _pango_emoji_iter_next (PangoEmojiIter *iter)
 {
   PangoEmojiType current_emoji_type = PANGO_EMOJI_TYPE_INVALID;
 
   if (iter->end == iter->text_end)
     return FALSE;
 
   iter->start = iter->end;
 
   for (; iter->end < iter->text_end; iter->end = g_utf8_next_char (iter->end))
     {
       gunichar ch = g_utf8_get_char (iter->end);
 
      
     if ((!(ch == kZeroWidthJoinerCharacter && !iter->is_emoji) &&
 	 ch != kVariationSelector15Character &&
 	 ch != kVariationSelector16Character &&
 	 ch != kCombiningEnclosingCircleBackslashCharacter &&
 	 !_pango_Is_Regional_Indicator(ch) &&
 	 !((ch == kLeftSpeechBubbleCharacter ||
 	    ch == kRainbowCharacter ||
 	    ch == kMaleSignCharacter ||
 	    ch == kFemaleSignCharacter ||
 	    ch == kStaffOfAesculapiusCharacter) &&
 	   !iter->is_emoji)) ||
 	current_emoji_type == PANGO_EMOJI_TYPE_INVALID) {
       current_emoji_type = _pango_get_emoji_type (ch);
     }
 
     if (g_utf8_next_char (iter->end) < iter->text_end)  
     {
       gunichar peek_char = g_utf8_get_char (g_utf8_next_char (iter->end));
 
        
       if (current_emoji_type ==
 	      PANGO_EMOJI_TYPE_EMOJI_EMOJI &&
 	  peek_char == kVariationSelector15Character) {
 	current_emoji_type = PANGO_EMOJI_TYPE_EMOJI_TEXT;
       }
 
       if ((current_emoji_type ==
 	       PANGO_EMOJI_TYPE_EMOJI_TEXT ||
 	   _pango_Is_Emoji_Keycap_Base(ch)) &&
 	  peek_char == kVariationSelector16Character) {
 	current_emoji_type = PANGO_EMOJI_TYPE_EMOJI_EMOJI;
       }
 
        
       if (_pango_Is_Emoji_Keycap_Base(ch) &&
 	  peek_char == kCombiningEnclosingKeycapCharacter) {
 	current_emoji_type = PANGO_EMOJI_TYPE_EMOJI_EMOJI;
       };
 
        
       if (_pango_Is_Regional_Indicator(ch) &&
 	  _pango_Is_Regional_Indicator(peek_char)) {
 	current_emoji_type = PANGO_EMOJI_TYPE_EMOJI_EMOJI;
       }
 
        
       if ((ch == kEyeCharacter ||
 	   ch == kWavingWhiteFlagCharacter) &&
 	  peek_char == kZeroWidthJoinerCharacter) {
 	current_emoji_type = PANGO_EMOJI_TYPE_EMOJI_EMOJI;
       }
     }
 
     if (iter->is_emoji == (gboolean) 2)
       iter->is_emoji = !PANGO_EMOJI_TYPE_IS_EMOJI (current_emoji_type);
      if (iter->is_emoji == PANGO_EMOJI_TYPE_IS_EMOJI (current_emoji_type))
      {
        iter->is_emoji = !PANGO_EMOJI_TYPE_IS_EMOJI (current_emoji_type);
// 
//        
//       if (iter->start == iter->end)
//         iter->end = g_utf8_next_char (iter->end);
// 
        return TRUE;
      }
    }
 
   iter->is_emoji = PANGO_EMOJI_TYPE_IS_EMOJI (current_emoji_type);
 
   return TRUE;
 }