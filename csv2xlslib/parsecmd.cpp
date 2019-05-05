/*
 * @file parsecmd.cpp
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

#include "parsecmd.hpp"
#include <iostream>
#include <filesystem>
#include "version.h"
#include "filename.hpp"
#include <sstream>
#include <regex>

namespace csv2xls
{
    using CmdInputFilePath            = CheckedCmd::Arg<InputFile>;
    using CmdPrintVersionInfo         = CheckedCmd::Flag<PrintVersionInfo>;
    using CmdInputHasHeadLine         = CheckedCmd::Flag<InputHasHeadLine>;
    using CmdCsvSeparatorIsTab        = CheckedCmd::Flag<CsvSeparatorIsTab>;
    using CmdOutputFilePath           = CheckedCmd::Param<std::optional<OutPutFile>>;
    using CmdXlsSheetName             = CheckedCmd::Param<std::optional<XlsSheetName>>;
    using CmdCsvSeparator             = CheckedCmd::Param<std::optional<CsvSeparator>>;
    using CmdOutPutRowLimit           = CheckedCmd::Param<std::optional<OutPutRowLimit>>;
    using CmdInputBufferSize          = CheckedCmd::Param<std::optional<InputBufferSize>>;
    using CmdOutPutFileNameDigitCount = CheckedCmd::Param<std::optional<OutPutFileNameDigitCount>>;

    auto MakeCmdConfig(){
        using CheckedCmd::Help;
        using CheckedCmd::Hint;
        using CheckedCmd::LongName;
        using CheckedCmd::ShortName;
        using CheckedCmd::Description;

        auto const IsValidSheetName = [](XlsSheetName const& xlsSheetName){
            return xlsSheetName.Get().size() < 32;
        };

        auto const isValidOutPutRowLimit = [](OutPutRowLimit const& outPutRowLimit){
            return (outPutRowLimit.Get() < DEFAULT_XLS_MIN_LINES.Get())? false:
                   (outPutRowLimit.Get() > DEFAULT_XLS_MAX_LINES.Get())? false:
                                                                         true;
        };

        auto const isValidInPutBufferSize = [](InputBufferSize const& inputBufferSize){
            return (inputBufferSize.Get() <  MIN_CSV_BUFFER_SIZE.Get())? false:
                   (inputBufferSize.Get() <= MAX_CSV_BUFFER_SIZE.Get())? true:
                                                                         false;
        };

        auto const isValidOutPutFileNameDigitCount = [](OutPutFileNameDigitCount const& out_put_file_name_digit_count){
            return (out_put_file_name_digit_count.Get() < 11);
        };

        auto const NoChecks = [](auto const& ){ return true;};

        return std::make_tuple(
                CmdInputFilePath(
                        Hint("inputfilename")
                        , Description("path to a CSV file to be converted to excel")
                        , NoChecks
                )

                ,CmdXlsSheetName(
                        Hint(R"("Table1")")
                        , ShortName("-w")
                        , LongName("--WorkSheetName")
                        , Description("Sets the name of the excel work sheet")
                        , IsValidSheetName
                )

                ,CmdCsvSeparator(
                        Hint(R"(";")")
                        , ShortName("-d")
                        , LongName("--Delimiter")
                        , Description("Character which is used to separate the columns in csv inputfile")
                        , NoChecks
                )

                ,CmdOutPutRowLimit(
                        Hint(std::to_string(DEFAULT_XLS_MIN_LINES.Get()) + ".." + std::to_string(DEFAULT_XLS_MAX_LINES.Get()))
                        , ShortName("-l")
                        , LongName("--LineLimit")
                        , Description("max. Number of lines in excel outputfile")
                        , isValidOutPutRowLimit
                )

                ,CmdOutputFilePath(
                        Hint("outputfilename")
                        , ShortName("-o")
                        , LongName("--OutPutFile")
                        , Description("name for the output excel file")
                        , NoChecks
                )

                ,CmdInputBufferSize(
                        Hint("N")
                        , ShortName("-b")
                        , LongName("--BufferSize")
                        , Description("Number of bytes used for input buffer to N")
                        , isValidInPutBufferSize
                )

                ,CmdPrintVersionInfo(
                        ShortName("-v")
                        , LongName("--Version")
                        , Description("Print program version information")
                )

                ,CmdInputHasHeadLine(
                        ShortName("-H")
                        , LongName("--HasHeadLine")
                        , Description("The first line of the input file will be the first line of all produced excel files")
                )

                ,CmdCsvSeparatorIsTab(
                        ShortName("-t")
                        , LongName("--Tab")
                        , Description(R"(This sets \t as column separator for csv input and overrides option '-d')")
                )

                ,CmdOutPutFileNameDigitCount(
                        Hint("N")
                        , ShortName("-D")
                        , LongName("--DigitCount")
                        , Description("If the excel file gets split, each file gets a number with min. N digits in its filename ")
                        , isValidOutPutFileNameDigitCount
                )
                ,Help()
        );

    }
    using CmdConfig = decltype(MakeCmdConfig());

    auto IntoConfig( CmdConfig cmdConfig)
    {

        Config config;

        if (std::get<CmdPrintVersionInfo>(cmdConfig).value().Get())
        {
            config.exit_clean = true;
            return config;
        }

        if (std::get<CheckedCmd::Help>(cmdConfig).value())
        {
            config.exit_clean = true;
            return config;
        }

        config.csv_file_name         = std::get<CmdInputFilePath>(cmdConfig).value();
        config.csv_file_has_headline = std::get<CmdInputHasHeadLine>(cmdConfig).value();

        if (std::get<CmdOutputFilePath>(cmdConfig).has_value())
        {
            auto const user_input = std::get<CmdOutputFilePath>(cmdConfig).value().Get();
            config.xls_file_name  = std::regex_replace(user_input.string(), std::regex("^ +"), "");
        }

        if (std::get<CmdCsvSeparatorIsTab>(cmdConfig).value().Get())
        {
            config.csv_tab_delimiter = CsvSeparator(CHAR_TABULATOR);
        }
        else if (std::get<CmdCsvSeparator>(cmdConfig).has_value())
        {
            config.csv_tab_delimiter = std::get<CmdCsvSeparator>(cmdConfig).value();
        }

        if (std::get<CmdXlsSheetName>(cmdConfig).has_value())
        {
            config.xls_sheet_name = std::get<CmdXlsSheetName>(cmdConfig).value();
        }

        if (std::get<CmdOutPutRowLimit>(cmdConfig).has_value())
        {
            config.xls_row_limit = std::get<CmdOutPutRowLimit>(cmdConfig).value();
        }

        if (std::get<CmdInputBufferSize>(cmdConfig).has_value())
        {
            config.input_buffer_size = std::get<CmdInputBufferSize>(cmdConfig).value();
        }

        if (std::get<CmdOutPutFileNameDigitCount>(cmdConfig).has_value())
        {
            config.xls_digit_count = std::get<CmdOutPutFileNameDigitCount>(cmdConfig).value();
        }

        return config;
    }

    using namespace std;
    bool isDir(string const& path);

    Config checkOptions(Config opts)
    {
        if (opts.exit_clean)
        {
            return opts;
        }

        if ((opts.csv_file_has_headline.Get()) && (opts.xls_row_limit.Get() < 2))
        {
            throw BadCommandLineOption("if first line is head line, then minimum line limit is 2");
        }

        return opts;
    }

    Config parse_commandline(int argc, char**argv)
    {
        auto const parser_result = CheckedCmd::ParseCmdArgsTuple(argc, argv, MakeCmdConfig());

        if (!parser_result)
        {
            throw BadCommandLineOption("error parsing command line");
        }

        if (std::get<CheckedCmd::Help>(parser_result.value()).value())
        {
            std::cout << std::get<CheckedCmd::Help>(parser_result.value()).GetDescription() << "\n";
        }

        if (std::get<CmdPrintVersionInfo>(parser_result.value()).value().Get())
        {
            std::cout << gGIT_VERSION << "\n";
        }

        return set_xls_filename(checkOptions(IntoConfig(parser_result.value())));

    }

    bool isDir(std::filesystem::path const& path)
    {
        return !path.has_filename();
    }

    Config set_xls_filename(Config opts)
    {
        if (opts.exit_clean) return opts;

        if (opts.xls_file_name.empty())
        {
            if (opts.csv_file_name.Get().has_filename())
            {
                opts.xls_file_name = opts.csv_file_name.Get().filename();
            }
            else
            {
                throw BadCommandLineOption("Error determining output file name");
            }
        }
        else if (isDir(opts.xls_file_name))
        {
            opts.xls_file_name /= opts.csv_file_name.Get().filename();
        }
        opts.xls_file_name = xls_filename(opts.xls_file_name, 0, OutPutFileNameDigitCount(0));
        return opts;
    }

    BadCommandLineOption::BadCommandLineOption(std::string const& what)
            : logic_error(what) {}
}/* -----  end of namespace csv2xls  ----- */
