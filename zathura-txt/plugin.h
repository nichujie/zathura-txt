/* SPDX-License-Identifier: Zlib */

#ifndef TXT_H
#define TXT_H

#include <stdbool.h>
#include <cairo.h>

#include <girara/macros.h>

#include <zathura/plugin-api.h>


typedef struct txt_document_s txt_document_t;
typedef struct txt_page_s txt_page_t;

/**
 * Opens a new document
 *
 * @param document The document
 * @return ZATHURA_ERROR_OK if no error occurred
 */
GIRARA_HIDDEN zathura_error_t txt_document_open(zathura_document_t* document);

/**
 * Frees the document
 *
 * @param document The document
 * @param data Custom data
 * @return ZATHURA_ERROR_OK if no error occurred
 */
GIRARA_HIDDEN zathura_error_t txt_document_free(zathura_document_t* document, void* txt_document);

/**
 * Initializes a page
 *
 * @param page The page
 * @return ZATHURA_ERROR_OK if no error occurred
 */
zathura_error_t txt_page_init(zathura_page_t* page);

/**
 * Clear page
 *
 * @param page The page
 * @param txt_page txt Page
 * @return ZATHURA_ERROR_OK if no error occurred
 */
GIRARA_HIDDEN zathura_error_t txt_page_clear(zathura_page_t* page, void* txt_page);

/**
 * Renders the page to a cairo object
 *
 * @param page The page
 * @param txt_page txt Page
 * @param cairo Cairo object
 * @param printing Render for printing
 * @return ZATHURA_ERROR_OK if no error occurred
 */
GIRARA_HIDDEN zathura_error_t txt_page_render_cairo(zathura_page_t* page, void* txt_page,
    cairo_t* cairo, bool printing);

#endif // TXT_H
