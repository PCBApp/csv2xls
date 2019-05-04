/*
 * @file filename.cpp
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

#include "filename.hpp"
#include <sstream>
#include <algorithm>
#include <regex>
#include <filesystem>
namespace csv2xls
{

using namespace std;

    string ConvertCountToStringWithLeadingZero(long numberOfDigits,
                                               long count)
    {
        stringstream numstream;
        numstream.width(numberOfDigits);
        numstream.fill('0');
        numstream << count;
        return numstream.str();
    }

    struct FileNameParts{
        FileNameParts() = default;
        FileNameParts(std::filesystem::path base_, std::filesystem::path type_)
                :base(std::move(base_)), type(std::move(type_)){}
        std::filesystem::path base;
        std::filesystem::path type;
    };

    FileNameParts SplitIntoParts(std::filesystem::path const& filename)
    {
        return FileNameParts(filename.parent_path() /= filename.stem(), filename.extension());
    }

    FileNameParts SetOutputFileNameParts(FileNameParts parts)
    {
        constexpr auto xls_txt = ".xls";

        if (parts.type.empty()||
            std::regex_match(parts.type.string(), std::regex(R"(\.csv$)", std::regex::icase)))
        {
            parts.type.assign(xls_txt);
        }
        else if(!std::regex_match(parts.type.string(), std::regex(R"(\.xls$)", std::regex::icase)))
        {
            parts.base = parts.base.string() + parts.type.string();
            parts.type.assign(xls_txt);
        }
        return parts;
    }

    auto AddNumberToBaseName(long count, long digits)
    {
        return [count, digits](FileNameParts parts){
            if (count && digits)
                parts.base += ConvertCountToStringWithLeadingZero(digits, count);
            return parts;
        };
    }

    std::filesystem::path BuildXlsFilename(FileNameParts parts)
    {
        return parts.base.string() + parts.type.string();
    }

    std::filesystem::path xls_filename(std::filesystem::path wish_name,
                        long count,
                        long digits)
    {
        return BuildXlsFilename(
                AddNumberToBaseName(count, digits)(
                        SetOutputFileNameParts(
                                SplitIntoParts(wish_name))));
    }

}/*---namespace csv2xls---*/
