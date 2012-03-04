#include "SQLiteConnection.h"
#include "SQLiteCountProxy.h"
#include "SQLiteExceptions.h"
#include "../DbConnectionFactory.h"

#include <sqlite3.h>

void finalize_sqlite3(sqlite3* db)
{
    // TODO: destructors cannot throw,
    // but we should report if ret != OK -> need logging
    if (db)
        sqlite3_close(db);
}

namespace dbc
{

SQLiteConnection::SQLiteConnection(const std::string& params) :
    _params(params),
    _db()
{
    sqlite3* db;
    int ret = sqlite3_open(params.c_str(), &db);

    // Whether or not an error occurs when it is opened, resources associated
    // with the database connection handle should be released by passing it to
    // sqlite3_close() when it is no longer required.
    _db.reset(db);

    if (ret != SQLITE_OK)
    {
        std::ostringstream msg;
        msg << "sqlite3_open(" << params << ") failed";
        throw SQLiteDbError(db, msg.str());
    }
}

CountProxy& SQLiteConnection::executeUpdate(const std::string& sql)
{
    static SQLiteCountProxy count(handle());

    int ret = sqlite3_exec(handle(), sql.c_str(), 0, 0, 0);
    if (ret != SQLITE_OK)
        throw SQLiteSqlError(*this, "sqlite3_exec() failed", sql);

    return count;
}

}