const GritResourceMap* GetKeyboardExtensionResources(size_t* size) {
  static const GritResourceMap kKeyboardResources[] = {
      {"keyboard/locales/en.js", IDR_KEYBOARD_LOCALES_EN},
      {"keyboard/config/m-emoji.js", IDR_KEYBOARD_CONFIG_EMOJI},
      {"keyboard/config/m-hwt.js", IDR_KEYBOARD_CONFIG_HWT},
      {"keyboard/config/us.js", IDR_KEYBOARD_CONFIG_US},
      {"keyboard/emoji.css", IDR_KEYBOARD_CSS_EMOJI},
      {"keyboard/images/backspace.png", IDR_KEYBOARD_IMAGES_BACKSPACE},
      {"keyboard/images/car.png", IDR_KEYBOARD_IMAGES_CAR},
      {"keyboard/images/check.png", IDR_KEYBOARD_IMAGES_CHECK},
      {"keyboard/images/compact.png", IDR_KEYBOARD_IMAGES_COMPACT},
      {"keyboard/images/down.png", IDR_KEYBOARD_IMAGES_DOWN},
      {"keyboard/images/emoji.png", IDR_KEYBOARD_IMAGES_EMOJI},
      {"keyboard/images/emoji_cat_items.png", IDR_KEYBOARD_IMAGES_CAT},
      {"keyboard/images/emoticon.png", IDR_KEYBOARD_IMAGES_EMOTICON},
      {"keyboard/images/enter.png", IDR_KEYBOARD_IMAGES_RETURN},
      {"keyboard/images/error.png", IDR_KEYBOARD_IMAGES_ERROR},
      {"keyboard/images/favorit.png", IDR_KEYBOARD_IMAGES_FAVORITE},
      {"keyboard/images/flower.png", IDR_KEYBOARD_IMAGES_FLOWER},
      {"keyboard/images/globe.png", IDR_KEYBOARD_IMAGES_GLOBE},
      {"keyboard/images/hide.png", IDR_KEYBOARD_IMAGES_HIDE_KEYBOARD},
      {"keyboard/images/keyboard.svg", IDR_KEYBOARD_IMAGES_KEYBOARD},
      {"keyboard/images/left.png", IDR_KEYBOARD_IMAGES_LEFT},
      {"keyboard/images/penci.png", IDR_KEYBOARD_IMAGES_PENCIL},
      {"keyboard/images/recent.png", IDR_KEYBOARD_IMAGES_RECENT},
      {"keyboard/images/regular_size.png", IDR_KEYBOARD_IMAGES_FULLSIZE},
      {"keyboard/images/menu.png", IDR_KEYBOARD_IMAGES_MENU},
      {"keyboard/images/pencil.png", IDR_KEYBOARD_IMAGES_PENCIL},
      {"keyboard/images/right.png", IDR_KEYBOARD_IMAGES_RIGHT},
      {"keyboard/images/search.png", IDR_KEYBOARD_IMAGES_SEARCH},
      {"keyboard/images/setting.png", IDR_KEYBOARD_IMAGES_SETTINGS},
      {"keyboard/images/shift.png", IDR_KEYBOARD_IMAGES_SHIFT},
      {"keyboard/images/space.png", IDR_KEYBOARD_IMAGES_SPACE},
      {"keyboard/images/tab.png", IDR_KEYBOARD_IMAGES_TAB},
      {"keyboard/images/triangle.png", IDR_KEYBOARD_IMAGES_TRIANGLE},
      {"keyboard/images/up.png", IDR_KEYBOARD_IMAGES_UP},
      {"keyboard/index.html", IDR_KEYBOARD_INDEX},
      {"keyboard/inputview_adapter.js", IDR_KEYBOARD_INPUTVIEW_ADAPTER},
      {"keyboard/inputview.css", IDR_KEYBOARD_INPUTVIEW_CSS},
      {"keyboard/inputview.js", IDR_KEYBOARD_INPUTVIEW_JS},
      {"keyboard/inputview_layouts/101kbd.js", IDR_KEYBOARD_LAYOUTS_101},
      {"keyboard/inputview_layouts/compactkbd-qwerty.js",
       IDR_KEYBOARD_LAYOUTS_COMPACT_QWERTY},
      {"keyboard/inputview_layouts/compactkbd-numberpad.js",
       IDR_KEYBOARD_LAYOUTS_COMPACT_NUMBERPAD},
      {"keyboard/inputview_layouts/emoji.js", IDR_KEYBOARD_LAYOUTS_EMOJI},
      {"keyboard/inputview_layouts/handwriting.js", IDR_KEYBOARD_LAYOUTS_HWT},
      {"keyboard/inputview_layouts/m-101kbd.js",
       IDR_KEYBOARD_LAYOUTS_MATERIAL_101},
      {"keyboard/inputview_layouts/m-compactkbd-qwerty.js",
       IDR_KEYBOARD_LAYOUTS_MATERIAL_COMPACT_QWERTY},
      {"keyboard/inputview_layouts/m-compactkbd-numberpad.js",
       IDR_KEYBOARD_LAYOUTS_MATERIAL_COMPACT_NUMBERPAD},
      {"keyboard/inputview_layouts/m-emoji.js",
       IDR_KEYBOARD_LAYOUTS_MATERIAL_EMOJI},
      {"keyboard/inputview_layouts/m-handwriting.js",
       IDR_KEYBOARD_LAYOUTS_MATERIAL_HWT},
      {"keyboard/manifest.json", IDR_KEYBOARD_MANIFEST},
      {"keyboard/sounds/keypress-delete.wav",
       IDR_KEYBOARD_SOUNDS_KEYPRESS_DELETE},
      {"keyboard/sounds/keypress-return.wav",
       IDR_KEYBOARD_SOUNDS_KEYPRESS_RETURN},
      {"keyboard/sounds/keypress-spacebar.wav",
       IDR_KEYBOARD_SOUNDS_KEYPRESS_SPACEBAR},
      {"keyboard/sounds/keypress-standard.wav",
       IDR_KEYBOARD_SOUNDS_KEYPRESS_STANDARD},
  };
  static const size_t kKeyboardResourcesSize = arraysize(kKeyboardResources);
  *size = kKeyboardResourcesSize;
  return kKeyboardResources;
}