#include <iostream>
#include <stdexcept>

class Row
{
    size_t length;
    int* data_row;
public:
    Row(int * begin_add, size_t len) :length(len), 
                                        data_row (begin_add) { }
    
    int& operator[] (size_t x)
    {
        if (x < length)
        {
            return data_row[x];
        }
        else
        {
            throw std::runtime_error("Uncorrect a[i]");
        }
    }

    const int& operator[] (size_t x) const
    {
        if (x < length)
        {
            return data_row[x];
        }
        else
        {
            throw std::runtime_error("Uncorrect a[i]");
        }
    }

    ~Row() { }
};


class Matrix
{
    size_t rows;
    size_t columns;
    int* data_matrix;
public:
    Matrix(const size_t rows, const size_t columns) : rows(rows),columns(columns),
                                                      data_matrix (new int[rows*columns])
    { }

    size_t getRows() const
    {
        return rows;
    }

    size_t getColumns() const
    {
        return columns;
    }

    const Row operator[](size_t i) const
    {
        if (i >= 0)
        {
            int* need_memory = new int[columns];
            for (int l=0; l < columns; l++)
            {
                need_memory[l] = data_matrix[i*columns + l];
            }
            Row res = Row(need_memory, columns);
            delete[] need_memory;
            return res;
        }
        else
        {
            throw std::runtime_error("a[.][]");
        }
    }

    Row operator[](size_t i)
    {
        if (i < rows)
        {
            Row res = Row(&data_matrix[i*columns], columns);
            return res;
        }
        else
        {
            throw std::runtime_error("a[.][]");
        }
    }

    Matrix& operator*=(size_t m)
    {
        for (size_t i = 0; i < rows*columns; i++)
            data_matrix[i] *= m;
        return *this;
    }

    bool operator==(const Matrix& other) const
    {
        //Проверка на корректность
        if ((rows != other.rows) || (columns != other.columns))
        {
            return false;
        }
        for (size_t i = 0; i < rows*columns; i++)
        {
            if (data_matrix[i] != other.data_matrix[i])
                return false;
        }
        return true;
    }

    bool operator!=(const Matrix& other) const
    {
        return !(*this == other);
    }

    ~Matrix()
    {
        delete[] data;
    }
};
