/*
 * @file csv.cpp
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
#include <cstdio>
#include <cstdlib>
#include <csv.h>

#include "csv.hpp"
#include "csv2xls_types.h"

namespace csv2xls
{
    void ParserDeleter::operator()(struct csv_parser *p){
        if(p) {
            csv_free(p);
        }
        delete p;
    }

    Parser::Parser(CsvSeparator tab_delimiter_)
            :csv_file_parser(std::unique_ptr<csv_parser, ParserDeleter>(new csv_parser()))
             ,tab_delimiter(tab_delimiter_)
    {
    }

    static int csv_is_space(unsigned char c)
    {
        return ((c == CSV_SPACE) || (c == CSV_TAB));
    }

    static int csv_is_term(unsigned char c)
    {
        return ((c == CSV_CR) || (c == CSV_LF));
    }

    Parser createParser(CsvSeparator tab_delimiter)
    {
        Parser csvin(tab_delimiter);

        constexpr unsigned char parser_options = (CSV_MAJOR >= 3) ? CSV_APPEND_NULL : 0;

        if (csv_init(csvin.csv_file_parser.get(), parser_options) != 0)
        {
            fprintf(stderr, "Failed to initialize csv parser\n");
            exit(EXIT_FAILURE);
        }

        csv_set_space_func(csvin.csv_file_parser.get(), csv_is_space);

        csv_set_term_func(csvin.csv_file_parser.get(), csv_is_term);

        csv_set_delim(csvin.csv_file_parser.get(), static_cast<unsigned char>(csvin.tab_delimiter.Get()));
        return  csvin;
    }

}/* ----- end of namespace csv2xls ----- */

