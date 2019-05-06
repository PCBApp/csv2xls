/**
 * @file callback.cpp:
 *
 * csv2xls - convert csv files into one or more Excel(TM) files
 * Copyright (C) 2012  Marcel Schneider
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU GENERAL PUBLIC LICENSE
 * as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU GENERAL PUBLIC LICENSE for more details.
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 *
 * Free Software Foundation Inc.
 * 51 Franklin Street
 * Fifth Floor
 * Boston
 * MA  02110-1301  USA
 */

#include "callback.hpp"
#include "XlsFile.hpp"
#include <csv.h>
namespace csv2xls
{
#if CSV_MAJOR < 3
    #define CSV2XLS_LEN len
#else
    #define CSV2XLS_LEN
#endif

void csv_cb_end_of_field(void *s, size_t CSV2XLS_LEN, void *data)
{
      auto *csv_field = (char*) s;
#if CSV_MAJOR < 3
      *(csv_field + CSV2XLS_LEN ) = '\0'; /*terminate string*/
#endif
    auto &xls_file = *(xls_file_t*) data;

    appendCell(xls_file, CellContent(csv_field));
}/* ----- end of function csv_cb_end_of_field ----- */

void csv_cb_headline_field(void *s, size_t CSV2XLS_LEN, void *data)
{
    auto *csv_field = (char*) s;
    auto &xls_file = *(xls_file_t*) data;

    if (XLS_MAX_COLUMNS.Get() >= xls_file.headline.size())
    {
#if  CSV_MAJOR < 3
        *(csv_field + CSV2XLS_LEN) = '\0'; /*terminate string*/
#endif
        appendCell(xls_file, CellContent(csv_field));
        xls_file.headline.emplace_back(csv_field);
    }
}/* ----- end of function csv_cb_headline_field ----- */

void csv_cb_end_of_row(int , void *data)
{
    auto &xls_file = *(xls_file_t*) data;
    newLine(xls_file);
}/* ----- end of function csv_cb_end_of_row ----- */
}/* ---- end of namespace csv2xls ---- */
