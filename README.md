mruby-odbc
=========
ODBC library for mruby.
experimental stage


Dependences
=========
- unixODBC

Example
=========

```
e = ODBC::Env.new
e.set_attr ODBC::Env::ODBC_VERSION, ODBC::Env::VERSION_3

conn = ODBC::Conn.new e

conn.driver_connect 'DSN=myodbc'

stmt = ODBC::Stmt.new conn

rs = stmt.exec_direct 'SELECT * FROM users'

while rs.next
  puts "col(1): #{rs.get_string 1}"
  puts "col(2): #{rs.get_string 2}"
end
```

License
=========
MIT License

