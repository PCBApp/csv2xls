/**
 * @file XlsFileTest.hpp
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
 * MA  02110-1301  USA *
 */
#ifndef XLS_FILE_TEST_H
#define XLS_FILE_TEST_H

#include "../src/XlsFile.hpp"
class dummy_workbook : public csv2xls::workbook
{
public:
             dummy_workbook(){called_clear_sheet   = 0;
                              called_write_to_file = 0;
                              called_label         = 0;  };
    virtual ~dummy_workbook(){};

    virtual void clear_sheet(const std::string& sheetname){called_clear_sheet++;}
    virtual int  write_to_file(const std::string &file_name){called_write_to_file++;return 0;}
    virtual void label(unsigned int row,
                       unsigned int col,
                       const std::string& strlabel){called_label++;}
    int called_clear_sheet;
    int called_write_to_file;
    int called_label;

};


    void read_CSV_into(csv2xls::xls_file_t *xlsfile,int row_count, int column_count);

#endif
