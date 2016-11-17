#ifndef MATRIX_H
#define MATRIX_H
#include <vector>
#include <iostream>

template<class T>
class Matrix;

template<class T>
std::ostream& operator<<(std::ostream &stream, const Matrix<T> &matrix);

template<class T>
class Matrix
{
public:
    Matrix(int rows, int cols);
    Matrix<T> slice(int xStart, int yStart, int xEnd, int yEnd) const;
    std::vector<T> all() const;

    void set(int x, int y, T object);
    T &get(int x, int y) const;
    int rows() const { return _rows; }
    int cols() const { return _cols; }
    friend std::ostream& operator<<<>(std::ostream &stream, const Matrix<T> &matrix);
private:
    int _rows;
    int _cols;
    std::vector<std::vector<T>> storage;
};


template<class T>
Matrix<T>::Matrix(int rows, int cols):
    _rows(rows), _cols(cols), storage(rows, std::vector<T>(cols))
{
}

template<class T>
Matrix<T> Matrix<T>::slice(int xStart, int yStart, int xEnd, int yEnd) const
{
    Matrix<T> matrix(xEnd-xStart, yEnd-yStart);
    for(int i=xStart; i < xEnd; i++) {
        for(int j=yStart; j < yEnd; j++) {
            matrix.set(i-xStart, j-yStart, storage[i][j]);
        }
    }
    return matrix;
}


template<class T>
std::vector<T> Matrix<T>::all() const
{
    std::vector<T> result(_rows*_cols);
    for(std::vector<T> &i: storage) {
        for(T &j: i) {
            result.push_back(j);
        }
    }
    return result;
}

template<class T>
T& Matrix<T>::get(int x, int y) const
{
    return storage[x][y];
}

template<class T>
void Matrix<T>::set(int x, int y, T object)
{
    storage[x][y] = object;
}

template<class T>
std::ostream& operator<<(std::ostream &stream, const Matrix<T> &matrix)
{
    for(auto &row: matrix.storage) {
        for(auto &cell: row) {
            stream << cell << " ";
        }
        stream << std::endl;

    }
    return stream;
}

#endif // MATRIX_H
