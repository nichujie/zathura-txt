
#include "plugin.h"
#include "internal.h"
#include "config.h"

zathura_error_t
txt_page_render_cairo(zathura_page_t* page, void* data,
    cairo_t* cairo, bool UNUSED(printing))
{
  txt_page_t* txt_page = (txt_page_t*)data;
  if (page == NULL || txt_page == NULL || cairo == NULL) {
    return ZATHURA_ERROR_INVALID_ARGUMENTS;
  }
  
  zathura_document_t* document = zathura_page_get_document(page);
  if (document == NULL) {
    return ZATHURA_ERROR_UNKNOWN;
  }
 
  cairo_set_source_rgb(cairo, 0.1, 0.1, 0.1);
 
  cairo_select_font_face(cairo,font_face,
      CAIRO_FONT_SLANT_NORMAL,
      CAIRO_FONT_WEIGHT_BOLD);
 
  cairo_set_font_size(cairo, font_size);
  cairo_text_extents_t ctet;
  int left = 40;
  int top = 40;
  // g_printf("<NEXT PAGE>\n");
  for (int i = 0; i < txt_page->line_count; i++) {
  //  g_printf("%s\n",txt_page->lines[i]);
    cairo_move_to(cairo, left, top);
    cairo_show_text(cairo, txt_page->lines[i]); 
    cairo_text_extents(cairo,txt_page->lines[i],&ctet);
    top += (line_spacing + ctet.height);
  }
 
  return ZATHURA_ERROR_OK;
}
