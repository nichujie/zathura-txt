/* SPDX-License-Identifier: Zlib */

#include <stdlib.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <gtk/gtk.h>

#include "plugin.h"
#include "internal.h"
#include "config.h"

zathura_error_t
txt_page_init(zathura_page_t* page)
{
  if (page == NULL) {
    return ZATHURA_ERROR_INVALID_ARGUMENTS;
  }

  zathura_document_t* document = zathura_page_get_document(page);
  txt_document_t* txt_document   = zathura_document_get_data(document);

  if (document == NULL || txt_document == NULL) {
    return ZATHURA_ERROR_UNKNOWN;
  }

  // Defined in [[plugin.h]]
  zathura_page_set_width(page, page_width);
  zathura_page_set_height(page, page_height);
  zathura_page_set_data(page, &txt_document->pages[zathura_page_get_index(page)]);

  return ZATHURA_ERROR_OK;
}

zathura_error_t
txt_page_clear(zathura_page_t* UNUSED(page), void* data)
{
  txt_page_t* txt_page = data;
  if (txt_page == NULL) {
    return ZATHURA_ERROR_OK;
  }

  for (int i = 0; i < txt_page->line_count; i++) {
    free(txt_page->lines[i]);
  }

  return ZATHURA_ERROR_OK;
}
