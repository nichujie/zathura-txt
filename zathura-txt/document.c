/* SPDX-License-Identifier: Zlib */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <glib/gstdio.h>
#include <girara/datastructures.h>

#include "plugin.h"
#include "internal.h"

#define MAX_LINES_PER_PAGE 46
#define MAX_CHARS_PER_LINE 84

static char** new_page();

zathura_error_t
txt_document_open(zathura_document_t* document)
{
  if (document == NULL) {
    return ZATHURA_ERROR_INVALID_ARGUMENTS;
  }

  txt_document_t* txt_document = g_malloc0(sizeof(txt_document_t));

  /* file path */
  const char* path = zathura_document_get_path(document);

  g_printf("Welcome to use NCJ's zathura txt plugin\n!");

  FILE* fp = fopen(path, "r");

  if (fp == NULL) {
    goto error_free;
  }

  txt_document->pages = calloc(1, sizeof(txt_page_t));
  txt_document->pages[0].lines = new_page();
  txt_document->pages[0].line_count = 0;

  if (txt_document->pages == NULL) goto error_free;

  char line[MAX_CHARS_PER_LINE + 1];
  int page_count = 0;

  while(fgets(line, MAX_CHARS_PER_LINE, fp)) {
    txt_document->pages[page_count].lines[txt_document->pages[page_count].line_count] = strdup(line);
    txt_document->pages[page_count].line_count++;

    if (txt_document->pages[page_count].line_count >= MAX_LINES_PER_PAGE) { 
      page_count++; 
      txt_document->pages = realloc(txt_document->pages, (page_count + 1) * sizeof(txt_page_t));
      txt_document->pages[page_count].lines = new_page();
      txt_document->pages[page_count].line_count = 0;
    }
  }

  /* set document information */
  zathura_document_set_number_of_pages(document, page_count + 1);
  zathura_document_set_data(document, txt_document);

  fclose(fp);

  return ZATHURA_ERROR_OK;

error_free:

  txt_document_free(document, txt_document);

  return ZATHURA_ERROR_UNKNOWN;
}

zathura_error_t
txt_document_free(zathura_document_t* UNUSED(document), void* data)
{
  txt_document_t* txt_document = data;
  if (txt_document == NULL) {
    return ZATHURA_ERROR_INVALID_ARGUMENTS;
  }

  /* remove page list */
  if (txt_document->pages != NULL) {
    free(txt_document->pages);
  }

  g_free(txt_document);

  return ZATHURA_ERROR_OK;
}

static char** 
new_page() 
{
  char **page = calloc(MAX_LINES_PER_PAGE, sizeof(char*));

  if (page == NULL) {
    return NULL;
  }

  return page;
}


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
 
  cairo_select_font_face(cairo,"Times New Roman",
      CAIRO_FONT_SLANT_NORMAL,
      CAIRO_FONT_WEIGHT_BOLD);
 
  cairo_set_font_size(cairo, 13);
 
  int left = 40;
  int top = 40;

  for (int i = 0; i < txt_page->line_count; i++) {
    cairo_move_to(cairo, left, top);
    cairo_show_text(cairo, txt_page->lines[i]); 
    top += 20;
  }
 
  return ZATHURA_ERROR_OK;
}

