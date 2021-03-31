#include "render/font/fconfig.h"

/*
  reference: https://gist.github.com/CallumDev/7c66b3f9cf7a876ef75f
*/

/*
 * Font pattern to font file
 * return string needs to be freed.
 */
char *match_font_desc(char *desc) {
  FcInit();
  FcConfig *config = FcInitLoadConfigAndFonts();

  FcPattern *pat = FcNameParse((const FcChar8 *)desc);

  FcConfigSubstitute(config, pat, FcMatchPattern);
  FcDefaultSubstitute(pat);

  char *font;
  FcResult result;

  FcPattern *fontm = FcFontMatch(config, pat, &result);
  if (fontm) {
    FcChar8 *val;
    if (FcPatternGetString(fontm, FC_FILE, 0, &val)) {
      log_error("failed to load font from \"%s\"", desc);
      return 0;
    }
    font = strdup((char *)(val));
  } else {
    log_error("failed to load font from \"%s\"", desc);
    return 0;
  }

  FcPatternDestroy(fontm);
  FcPatternDestroy(pat);
  FcConfigDestroy(config);
  FcFini();
  return font;
}
