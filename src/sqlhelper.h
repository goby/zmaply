/*
 *  sqlhelper.h
 *  zmaply
 *
 *  Reader and writer of SQLite
 *
 */

#include <sqlite3.h>
#include <string.h>

namespace zmaply {

/// execute one sql statement with resource safety,
/// nothing returned on success, exception thrown
/// on failure.
void execSqlOnce(sqlite3 *db,const char *stmtStr);

/** 
 * Encapsulates a SQLite3 statement in a way that does not make me
 * want to punch someone.
 **/
class StatementReader
{
public:
  /// Construct with the statement and maybe just run the damn thing
  StatementReader(sqlite3 *db,const char *,bool justRun=false);
  /// Destructor will call finalize
  ~StatementReader();
  
  /// Returns false if initialization failed
  bool isValid();
  
  /// Calls step, expecting a row.
  /// Returns false if we're done, throws an exception on error
  bool stepRow();

  /// You can force a finalize here
  void finalize();
        
  /// Return an int from the current row
  int getInt();
  /// Return a double from the current row
  double getDouble();        
  /// Return an NSString from the current row
  std::string *getString();
  /// Return a boolean from the current row
  BOOL getBool();
  /// Return a blob from the current row
  Blob *getBlob();
        
protected:
  void init(sqlite3 *db,const char *,bool justRun=false);
        
  bool valid;
  sqlite3 *db;
  sqlite3_stmt *stmt;
  bool isFinalized;
  int curField;
};

class StatementWriter
{
public:
  StatementWriter(sqlite3 *db,const char *);
  ~StatementWriter();
  
  /// Run the insert/update.
  /// Triggers an exception on failure
  void go();
        
  /// Finalize it (optional)
  void finalize();
        
  /// Add an integer
  void add(int);
  /// Add a double
  void add(double);
  /// Add a string
  void add(std::string *);
   /// Add a boolean
  void add(BOOL);
        
protected:
  void init(sqlite3 *db,const char *);

  sqlite3_stmt *stmt;
  bool isFinalized;
  int curField;
};        

}
