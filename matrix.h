#ifndef MATRIX_H
#define MATRIX_H
#include <vector>
#include <iostream>
#include <iterator>
#include <cassert>


/**
 * @brief A simple fixed-size matrix which allows read/write access to objects inside it.
 *
 * @tparam T Type of element contained in the matrix
 *
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
    Matrix(int rows, int cols);

    /**
     * @brief Creates a new matrix from a part of the matrix
     * @param xStart Start row to copy to the new matrix
     * @param yStart Start column to copy to the new matrix
     * @param xEnd End row to copy to the new matrix
     * @param yEnd End column to copy to the new matrix
     * @return A new matrix comprising a part of the complete matrix
     */
    Matrix<T> slice(int xStart, int yStart, int xEnd, int yEnd);

    /**
     * @brief Gets a list of all elements in the matrix
     * @return A vector containing all elements present in the matrix
     */
    std::vector<T> all() const;

    /**
     * @brief Updates a cell in the matrix
     * @param x Row position to place the element on
     * @param y Column position to place the element on
     * @param object The element to place in the matrix
     */
    void set(int x, int y, const T& object);

    /**
     * @brief Fetches a cell from the matrix
     * @param x Row position to fetch the element from
     * @param y Column position to fetch the element from
     * @return The element at that position in the matrix
     */
    T &get(int x, int y);

    /**
     * @brief Fetches a cell from the matrix
     * @param x Row position to fetch the element from
     * @param y Column position to fetch the element from
     * @return The element at that position in the matrix
     */
    const T &get(int x, int y) const;

    /**
     * @return The number of rows in this matrix
     */
    int rows() const { return _rows; }
    /**
     * @return The number of columns in this matrix
     */
    int cols() const { return _cols; }
private:
    inline void __verify_offsets(int x, int y) const;
    int _rows;
    int _cols;
    std::vector<T> storage;
};


template<class T>
Matrix<T>::Matrix(int xLen, int yLen):
    _rows(xLen), _cols(yLen), storage(xLen*yLen)
{
}

template<class T>
Matrix<T> Matrix<T>::slice(int xStart, int yStart, int xEnd, int yEnd)
{
    __verify_offsets(xStart, yStart);
    __verify_offsets(xEnd, yEnd);
    assert(xStart <= xEnd);
    assert(yStart <= yEnd);
    Matrix<T> matrix(xEnd-xStart+1, yEnd-yStart+1);
    for(int i=xStart; i <= xEnd; i++) {
        for(int j=yStart; j <= yEnd; j++) {
            matrix.set(i-xStart, j-yStart, this->get(i, j));
        }
    }
    return matrix;
}


template<class T>
std::vector<T> Matrix<T>::all() const
{
    return storage;
}

template<class T>
T& Matrix<T>::get(int x, int y)
{
    __verify_offsets(x, y);
    return storage[x*_cols+y];
}

template<class T>
const T& Matrix<T>::get(int x, int y) const
{
    __verify_offsets(x, y);
    return storage[x*_cols+y];
}

template<class T>
void Matrix<T>::set(int x, int y, const T& object)
{
    __verify_offsets(x, y);
    storage[x*_cols+y] = object;
}


template<class T>
inline void Matrix<T>::__verify_offsets(int x, int y) const
{
    assert(x >= 0);
    assert(y >= 0);
    assert(x < this->rows());
    assert(y < this->cols());
}

template<class T>
std::ostream& operator<<(std::ostream &stream, const Matrix<T> &matrix)
{
    for(int i=0; i < matrix.rows(); i++) {
        for(int j=0; j < matrix.cols(); j++) {
            stream << matrix.get(i, j);
            if(j < matrix.cols() - 1)
                stream << " ";
        }
        if (i < matrix.rows() - 1)
            stream << std::endl;
    }
    return stream;
}

#endif // MATRIX_H
