mruby-odbc
=========
unixODBC bindings for mruby.
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

conn.connect 'DSN=myodbc'

stmt = ODBC::Stmt.new conn

rs = stmt.execute 'SELECT * FROM users'

rs.each do |row|
  puts "col(1): #{rs[1]}"
  puts "col(2): #{rs[2]}"
end

```

License
=========
MIT License

