#ifndef MATRIX_H
#define MATRIX_H
#include <vector>
#include <map>
#include <iostream>
#include <iterator>
#include <cassert>
#include <memory>

template<class T>
class MatrixView;

template<class T>
class MatrixIterator;

/**
 * @brief A simple fixed-size matrix which allows read/write access to objects inside it.
 *
 * @tparam T Type of element contained in the matrix
 */
template<class T>
class Matrix
{
public:
    /**
     * @brief Creates a new matrix with a fixed size
     * @param rows The number of rows in the matrix
     * @param cols The number of columns in the matrix
     */
    Matrix(int rows, int cols):
        _rows(rows), _cols(cols)
    {

    }

    /**
     * @brief Checks if the matrix contains an element at this position
     * @param row Row position of the element
     * @param col Column position of the element
     * @return Whether there is an element present at this position
     */
    virtual bool contains(int row, int col) const
    {
        return row >= 0 && row < _rows && col >= 0 && col < _cols;
    }

    /**
     * @brief Fetches a cell from the matrix
     * @param row Row position to fetch the element from
     * @param col Column position to fetch the element from
     * @return The element at that position in the matrix
     */
    inline T& get(int row, int col)
    {
        assert(this->contains(row, col));
        return this->_get(row, col);
    }

    /**
     * @brief Updates a cell in the matrix
     * @param row Row position to place the element on
     * @param col Column position to place the element on
     * @param object The element to place in the matrix
     */
    inline void set(int row, int col, const T& object)
    {
        this->__verify_offsets(row, col);
        this->_set(row, col, object);
    }

    /**
     * @brief Creates a new matrix from a part of the matrix.
     * @param rowStart Start row to copy to the new matrix
     * @param colStart Start column to copy to the new matrix
     * @param rowEnd End row to copy to the new matrix
     * @param colEnd End column to copy to the new matrix
     * @return A view into the current matrix comprising a part of the complete matrix
     */
    std::unique_ptr<MatrixView<T>> slice(int rowStart, int colStart, int rowEnd, int colEnd)
    {
        this->__verify_offsets(rowStart, colStart);
        this->__verify_offsets(rowEnd, colEnd);
        assert(rowStart <= rowEnd);
        assert(colStart <= colEnd);
        return this->unsafeSlice(rowStart, colStart, rowEnd, colEnd);
    }

    virtual std::unique_ptr<MatrixView<T>> unsafeSlice(int rowStart, int colStart, int rowEnd, int colEnd)
    {
        return std::unique_ptr<MatrixView<T>>(new MatrixView<T>(this, rowStart, colStart, rowEnd, colEnd));
    }

    /**
     * @brief Returns an iterator to the first element of the matrix
     * @return An iterator pointing to the beginning of the matrix
     */
    virtual MatrixIterator<T> begin()
    {
        return MatrixIterator<T>(this, 0, 0);
    }

    /**
     * @brief Returns an iterator to the element following the last element of the matrix
     * @return An iterator pointing past the end of the matrix
     */
    virtual MatrixIterator<T> end()
    {
        return MatrixIterator<T>(this, this->rows(), 0);
    }

    /**
     * @return The number of rows in this matrix
     */
    inline int rows() const
    {
        return _rows;
    }

    /**
     * @return The number of columns in this matrix
     */
    inline int cols() const
    {
        return _cols;
    }

protected:
    virtual T& _get(int row, int col)  = 0;

    virtual void _set(int row, int col, const T& object) = 0;

    /**
     * @brief Asserts that a given position is within the bounds of the matrix
     * @param row
     * @param col
     */
    inline void __verify_offsets(int row, int col) const
    {
        assert(row >= 0);
        assert(col >= 0);
        assert(row < this->rows());
        assert(col < this->cols());
    }

private:
    int _rows;
    int _cols;
};


/**
 * @brief A dense matrix allocates a block of memory to contain all cells upfront.
 *
 * @tparam T Type of element contained in the matrix
 */
template<class T>
class DenseMatrix: public Matrix<T>
{
public:
    DenseMatrix(int rows, int cols):
        Matrix<T>(rows, cols), storage(rows*cols)
    {

    }

protected:
    virtual T &_get(int row, int col) override
    {
        return storage[row*this->cols()+col];
    }

    virtual void _set(int row, int col, const T& object) override
    {
        storage[row*this->cols()+col] = object;
    }

private:
    std::vector<T> storage;
};


/**
 * @brief A matrix view allows read/write access to a subset of a matrix.
 * @internal
 * @tparam T Type of element contained in the matrix
 */
template<class T>
class MatrixView: public Matrix<T>
{
public:
    MatrixView(Matrix<T> *matrix, int rowStart, int colStart, int rowEnd, int colEnd):
        Matrix<T>(rowEnd-rowStart+1, colEnd-colStart+1), matrix(matrix), _rowStart(rowStart), _colStart(colStart)
    {

    }

    virtual std::unique_ptr<MatrixView<T>> unsafeSlice(int rowStart, int colStart, int rowEnd, int colEnd)
    {
        return matrix->unsafeSlice(_rowStart+rowStart, _colStart+colStart, _rowStart+rowEnd, _colStart+colEnd);
    }

    virtual bool contains(int row, int col) const override
    {
        if(!Matrix<T>::contains(row, col))
            return false;
        return matrix->contains(_rowStart+row, _colStart+col);
    }

protected:
    virtual T &_get(int row, int col) override
    {
        return matrix->get(_rowStart+row, _colStart+col);
    }

    virtual void _set(int row, int col, const T& object) override
    {
        matrix->set(_rowStart+row, _colStart+col, object);
    }

private:
    Matrix<T> *matrix;
    int _rowStart;
    int _colStart;
};


/**
 * @brief An iterator that iterates over the elements in the matrix
 * @internal
 * @tparam T Type of element contained in the matrix
 */
template<class T>
class MatrixIterator
{
public:
    MatrixIterator(Matrix<T> *matrix, int rowPos, int colPos):
        matrix(matrix), colPos(colPos), rowPos(rowPos)
    {
        // If the first given position is not in the matrix, and we are still in bounds go to the next position
        if(!matrix->contains(rowPos, colPos) && rowPos < matrix->rows())
            this->operator ++();
    }

    bool operator!=(const MatrixIterator<T> &other) const
    {
        return matrix != other.matrix || colPos != other.colPos || rowPos != other.rowPos;
    }

    virtual MatrixIterator<T>& operator++()
    {
        do {
            ++colPos;
            if(colPos >= matrix->cols()) {
                colPos=0;
                ++rowPos;
            }
        /*
         * Keep going until we find a position that has an element
         * To prevent unbounded iteration, keep going as long as we are in a valid row.
         * When we reach matrix->rows(), we are one position behind the iterator,
         * so it nicely matches the end() iterator.
         */
        } while(!matrix->contains(rowPos, colPos) && rowPos < matrix->rows());
        return *this;
    }

    const T& operator*() const
    {
        return matrix->get(rowPos, colPos);
    }

protected:
    Matrix<T> *matrix;
    int colPos;
    int rowPos;
};


/**
 * @brief Prints a matrix to an output stream
 */
template<class T>
std::ostream& operator<<(std::ostream &stream, const Matrix<T> &matrix)
{
    for(int i=0; i < matrix.rows(); i++) {
        for(int j=0; j < matrix.cols(); j++) {
            if(matrix.contains(i, j)) {
                stream << matrix.get(i, j);
            } else {
                stream << '?';
            }
            if(j < matrix.cols() - 1)
                stream << " ";
        }
        if (i < matrix.rows() - 1)
            stream << std::endl;
    }
    return stream;
}

#endif // MATRIX_H
