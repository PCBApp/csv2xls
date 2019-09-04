//
// Created by marcel on 12/20/17.
//

#include "OutputDoc.hpp"
namespace csv2xls
{

int OutputDoc::writeInto(OutputFileName const& out_put_file, FileNumber file_number) const
{
    if(isEmpty())
        return 0;
    return self_->writeInto(out_put_file, file_number);
}
void OutputDoc::set(InputHasHeadLine input_has_head_line)
{
    m_input_has_head_line = input_has_head_line;
}

bool OutputDoc::isEmpty()const
{
    return (m_input_has_head_line.Get())
            ? (m_row == Row(1))&&(m_column == Column(0))
            : (m_row == Row(0))&&(m_column == Column(0));
}

Column OutputDoc::appendCell(CellContent cell_content)
{
    if (output_column_limit && m_column.isGreaterEqual (output_column_limit.value()))
    {
        return m_column;
    }
    self_->setCell(m_row, m_column, cell_content);
    ++m_column;
    return m_column;
}
Row OutputDoc::newLine()
{
    ++m_row;
    m_column = Column(0);
    return m_row;
}
}