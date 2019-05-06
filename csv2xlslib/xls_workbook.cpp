/**
 * @file xls_workbook.cpp
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

#include "xls_workbook.hpp"
#include <xlslib.h>
namespace csv2xls
{
struct xls_workbook::Impl{
    std::unique_ptr<xlslib_core::workbook> wbook;
    xlslib_core::worksheet* wsheet;
};

void xls_workbook::clearSheet(XlsSheetName const& sheet_name)
{
    this->pimpl->wbook  = std::make_unique<xlslib_core::workbook>();
    this->pimpl->wsheet = this->pimpl->wbook->sheet(sheet_name.Get());
}

void xls_workbook::setCell(Row row, Column column, CellContent const& cell_content)
{
    this->pimpl->wsheet->label(row.Get(), column.Get(), cell_content.Get());
}

int xls_workbook::writeInto(OutputFileName const& out_put_file)
{
    return this->pimpl->wbook->Dump(out_put_file.Get().string());
}

xls_workbook::xls_workbook()
    : pimpl(new Impl, &PimplDeleter)
{
}

xls_workbook::~xls_workbook() {}
void xls_workbook::PimplDeleter(Impl* p)
{
    delete p;
}

}
