//
// Created by marcel on 12/20/17.
//

#ifndef CSV2XLS_OUTPUTDOC_H
#define CSV2XLS_OUTPUTDOC_H

#include "csv2xls_types.h"
#include <filesystem>
#include <memory>
#include <string>
namespace csv2xls
{
class OutPutDoc
{
  public:
    template <typename T>
    explicit OutPutDoc(T&& x)
        : self_(new doc_type<T>(std::forward<T>(x)))
    {
    }

    void clear_sheet(const std::string& sheetname);

    int write_to_file(const std::filesystem::path& file_name);

    void setCell(Row row, Column column, const std::string& strlabel);

  private:
    class concept_t
    {
      public:
        virtual ~concept_t() = default;

        virtual void clear_sheet(const std::string& sheetname)                    = 0;
        virtual int  write_to_file(const std::filesystem::path& file_name)        = 0;
        virtual void setCell(Row row, Column column, const std::string& strlabel) = 0;
    };
    template <typename T> class doc_type : public concept_t
    {
      public:
        doc_type(T x_)
            : x(std::move(x_))
        {
        }

        void clear_sheet(const std::string& sheetname) override
        {
            x.clear_sheet(sheetname);
        }

        int write_to_file(const std::filesystem::path& file_name) override
        {
            return x.write_to_file(file_name);
        }

        void setCell(Row row, Column column, const std::string& strlabel) override
        {
            x.setCell(row, column, strlabel);
        }

      private:
        T x;
    };

  private:
    std::unique_ptr<concept_t> self_;
};
}

#endif //CSV2XLS_OUTPUTDOC_H
