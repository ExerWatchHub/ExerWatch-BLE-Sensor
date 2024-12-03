#pragma once
#ifndef UTILS_H
#define UTILS_H

#include "libs.h"


void bytes_to_str(int bytes, char *size_str)
{
  int mb = 1024 * 1024;
  int kb = 1024;
  if (bytes >= mb)
  {
    sprintf(size_str, "%3.1f MB", (float)bytes / mb);
  }
  else if (bytes >= kb)
  {
    sprintf(size_str, "%3.1f KB", (float)bytes / kb);
  }
  else
  {
    sprintf(size_str, "%3.1f B", (float)bytes);
  }
}

#endif
