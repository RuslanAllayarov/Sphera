class Matrix
{
    size_t rows;
    size_t columns;
    int** data_matrix;
    class Row
    {
        size_t length;
        int* data_row;
    public:
        Row(int* begin_add, size_t len) :length(len), 
                                         data_row (begin_add) { }    
        int& operator[] (size_t x)
        {
            if (x >= length)
                throw std::runtime_error("Uncorrect a[i]");
            return data_row[x];
        }
        const int& operator[] (size_t x) const
        {
            if (x >= length)
                throw std::runtime_error("Uncorrect a[i]");
            return data_row[x];
        }
        ~Row() { }
    };
public:
    Matrix(const size_t rows, const size_t columns) : rows(rows),columns(columns),
                                                        data_matrix(new int*[rows])
    {
        for(size_t i = 0; i < rows; i++)
            data_matrix[i] = new int[columns]();
    }
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
        if (i >= rows)
            throw std::runtime_error("a[.][]");
        return Row(data_matrix[i], columns);
    }
    Row operator[](size_t i)
    {
        if (i >= rows)
            throw std::runtime_error("a[.][]");
        return Row(data_matrix[i], columns);
    }
    Matrix& operator*=(size_t m)
    {
        for (size_t i = 0; i < rows; i++)
            for(size_t j = 0; j < columns; j++)
                data_matrix[i][j] *= m;
        return *this;
    }
    bool operator==(const Matrix& other) const
    {
        if ((rows != other.rows) || (columns != other.columns))
        {
            return false;
        }
        for (size_t i = 0; i < rows; i++)
            for (size_t j = 0; j < columns; j++)
                if (data_matrix[i][j] != other.data_matrix[i][j])
                    return false;
        return true;
    }
    bool operator!=(const Matrix& other) const
    {
        return !(*this == other);
    }
    ~Matrix()
    {
        for (size_t i = 0; i < rows; i++)
            delete[] data_matrix[i];
        delete[] data_matrix;
    }
};
