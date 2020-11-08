/* SPDX-License-Identifier: Zlib */

#ifndef INTERNAL_H
#define INTERNAL_H

#define LIBARCHIVE_BUFFER_SIZE 8192 

struct txt_document_s {
  struct txt_page_s* pages;
};

struct txt_page_s {
  int line_count;
  char **lines;
};

#endif // INTERNAL_H
