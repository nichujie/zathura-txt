/* SPDX-License-Identifier: Zlib */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <glib/gstdio.h>
#include <girara/datastructures.h>
#include <wchar.h>

#include "plugin.h"
#include "internal.h"
#include "config.h"

static char **new_page(int size);

zathura_error_t
txt_document_open(zathura_document_t *document)
{
  if (document == NULL)
  {
    return ZATHURA_ERROR_INVALID_ARGUMENTS;
  }

  txt_document_t *txt_document = g_malloc0(sizeof(txt_document_t));

  /* file path */
  const char *path = zathura_document_get_path(document);

  g_printf("Welcome to use NCJ's zathura txt plugin!\n");

  FILE *fp = fopen(path, "r");

  if (fp == NULL)
  {
    goto error_free;
  }

  txt_document->pages = calloc(1, sizeof(txt_page_t));

  if (txt_document->pages == NULL)
    goto error_free;

  // Create a cairo to measure font width
  cairo_t *cr = cairo_create(cairo_image_surface_create(CAIRO_FORMAT_A1, 1, 1));
  cairo_select_font_face(cr, font_face,
                         CAIRO_FONT_SLANT_NORMAL,
                         CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_font_size(cr, font_size);
  cairo_text_extents_t ctet;

  wchar_t c[128];
  char **lines = malloc(4 * 64);
  memset(c, '\0', sizeof(c));

  txt_document->pages = calloc(1, sizeof(txt_page_t));

  int char_index = 0, page_index = 0, line_index = 0;
  int left = 40, top = 40;
  int total_line_height = 0;

  while ((*(c + char_index) = fgetwc(fp)) != WEOF)
  {

    char tmp[sizeof(c) + 1];
    memset(tmp, '\0', sizeof(c) + 1);
    wcstombs(tmp, c, sizeof(wchar_t) * wcslen(c));

    cairo_text_extents(cr, tmp, &ctet);
    if (ctet.width > page_width - 2 * left || *(c + char_index) == '\n')
    {
      // g_printf("%s|", tmp);
      lines[line_index] = strdup(tmp);

      memset(c, '\0', sizeof(c));
      char_index = 0;
      line_index++;
      if (total_line_height + ctet.height + line_spacing < page_height - 2 * top)
      {
                

        total_line_height += ctet.height + line_spacing;
      }
      else
      {
        // 换行并换页
        // g_printf("<NEXT PAGE>\n");
        txt_document->pages[page_index].line_count = line_index;
        txt_document->pages[page_index].lines = new_page(line_index + 1);
        memcpy(txt_document->pages[page_index].lines, lines, line_index * sizeof(char *));
        memset(lines, NULL, sizeof(lines));

        total_line_height = 0;
        line_index = 0;
        page_index++;

        txt_document->pages = realloc(txt_document->pages, (page_index + 1) * sizeof(txt_page_t));
      }
    }
    else
    {
      char_index++;
    }
  }

  txt_document->pages[page_index].line_count = line_index;
  txt_document->pages[page_index].lines = new_page(line_index + 1);
  memcpy(txt_document->pages[page_index].lines, lines, line_index  * sizeof(char *));

  /* set document information */
  zathura_document_set_number_of_pages(document, page_index + 1);
  zathura_document_set_data(document, txt_document);

  fclose(fp);

  return ZATHURA_ERROR_OK;

error_free:

  txt_document_free(document, txt_document);

  return ZATHURA_ERROR_UNKNOWN;
}

zathura_error_t
txt_document_free(zathura_document_t *UNUSED(document), void *data)
{
  txt_document_t *txt_document = data;
  if (txt_document == NULL)
  {
    return ZATHURA_ERROR_INVALID_ARGUMENTS;
  }

  /* remove page list */
  if (txt_document->pages != NULL)
  {
    free(txt_document->pages);
  }

  g_free(txt_document);

  return ZATHURA_ERROR_OK;
}

static char **
new_page(int size)
{
  char **page = calloc(size, sizeof(char *));

  if (page == NULL)
  {
    return NULL;
  }

  return page;
}
