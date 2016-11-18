#ifndef MATRIX_H
#define MATRIX_H
#include <vector>
#include <iostream>
#include <iterator>
#include <cassert>


template<class T>
class Matrix
{
public:
    Matrix(int rows, int cols);
    Matrix<T> slice(int xStart, int yStart, int xEnd, int yEnd);
    std::vector<T> all() const;

    void setRef(int x, int y, T& object);
    void set(int x, int y, T object);
    T &get(int x, int y);
    const T &get(int x, int y) const;
    int rows() const { return _rows; }
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
    assert(xStart < xEnd);
    assert(yStart < yEnd);
    Matrix<T> matrix(xEnd-xStart, yEnd-yStart);
    for(int i=xStart; i < xEnd; i++) {
        for(int j=yStart; j < yEnd; j++) {
            matrix.setRef(i-xStart, j-yStart, this->get(i, j));
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
void Matrix<T>::set(int x, int y, T object)
{
    __verify_offsets(x, y);
    storage[x*_cols+y] = object;
}

template<class T>
void Matrix<T>::setRef(int x, int y, T& object)
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
