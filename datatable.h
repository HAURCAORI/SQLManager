#ifndef DataTable_H
#define DataTable_H
#include <vector>

template<typename T, T N = T(),  template <typename E, typename = std::allocator<E>> class Container = std::vector>
class DataTable {
public:
    using tableType = Container<Container<T>>;
    using arrayType = Container<T>;
    using valueType = T;

    DataTable();
    DataTable(size_t row, size_t col);
    DataTable(const DataTable& t);
    DataTable& operator=(const DataTable& t);
    DataTable(const DataTable&& t) noexcept;
    DataTable& operator=(const DataTable&& t) noexcept;
    ~DataTable();

    valueType& at(size_t row, size_t col);
    const valueType& at(size_t row, size_t col) const;

    arrayType& operator[](size_t row);
    const arrayType& operator[](size_t row) const;

    arrayType& getRow(size_t index);
    const arrayType& getRow(size_t index) const;
    arrayType& getColumn(size_t index);
    const arrayType& getColumn(size_t index) const;

    void addRow(const arrayType& items);
    void addColumn(const arrayType& items);

    void addRow(std::initializer_list<valueType> items);
    void addColumn(std::initializer_list<valueType> items);

    void removeRow(size_t index);
    void removeColumn(size_t index);

    size_t getRowCount() const;
    size_t getColumnCount() const;

    tableType& getTable();
    const tableType& getTable() const;

    void fill(valueType item);
    void insert(size_t row, size_t col, valueType item);

    void clear();
    void resize(size_t row, size_t col);
    //friend void swap(Table& t1,Table& t2) noexcept;

private:
    size_t mRowCount;
    size_t mColumnCount;
    tableType mTable;

    static const size_t defaultRowCount = 10;
    static const size_t defaultColumnCount = 10;
};

#endif // TABLE_H
