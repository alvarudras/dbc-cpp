#ifndef DBCCPP_RECORDSET_H__
#define DBCCPP_RECORDSET_H__

#include <utilcpp/declarations.h>
#include <dbccpp/DbExceptions.h>

#include <string>

#if defined(__GXX_EXPERIMENTAL_CXX0X__) || (__cplusplus > 199711L)
  #include <memory>
  namespace stdutil = std;
#else
  #include <boost/smart_ptr/shared_ptr.hpp>
  namespace stdutil = boost;
#endif

namespace dbc
{

/** Interface for representing query result sets. Resembles JDBC ResultSets,
 * but does not support updates.
 *
 * @throw DbException
 */
class ResultSet
{
    UTILCPP_DECLARE_INTERFACE(ResultSet)

public:

    typedef stdutil::shared_ptr<ResultSet> ptr;

    /** Moves the cursor froward one row from its current position.
     *
     * A ResultSet cursor is initially positioned before the first row; the
     * first call to the method next makes the first row the current row; the
     * second call makes the second row the current row, and so on.
     *
     * When a call to the next method returns false, the cursor is positioned
     * after the last row. Any invocation of a ResultSet method which requires
     * a current row will result in a SqlError being thrown.
     */
    virtual bool next() = 0;

    /** Returns true if the value of the designated column in the current row
     * of this ResultSet object is NULL.
     *
     * Checking for null works correctly *only before* the column is accessed
     * with get().
     */
    virtual bool isNull(int columnIndex) = 0;

    /** Retrieves the value of the designated column in the current row of
     * this ResultSet object.
     *
     * Returns default values (0 for ints, blanks for strings etc) for NULLs,
     * use isNull() for explicit NULL checking.
     */
    template <typename T>
    T get(int columnIndex);

    template <typename T>
    void get(int columnIndex, T& out);

protected:
    // NVI for get()
    virtual void getString(int columnIndex, std::string& out) = 0;
    virtual int getInt(int columnIndex) = 0;
    virtual double getDouble(int columnIndex) = 0;
    virtual bool getBool(int columnIndex) = 0;
};

}

#endif /* RECORDSET_H */
