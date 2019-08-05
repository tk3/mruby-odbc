puts "mruby-odbc version: #{ODBC::VERSION}"

env = ODBC::Env.new
env.set_attr ODBC::Env::ODBC_VERSION, ODBC::Env::VERSION_3

conn = ODBC::Conn.new env
conn.connect 'DSN=myodbc'

stmt = ODBC::Stmt.new conn
stmt.execute 'DROP TABLE IF EXISTS foo'
stmt.execute <<END_OF_SQL
CREATE TABLE foo (
  id int,
  name text
)
END_OF_SQL

stmt = ODBC::Stmt.new conn
stmt.execute "insert into foo values(1, 'aaa')"
stmt.execute "insert into foo values(2, 'bbb')"
stmt.execute "insert into foo values(3, 'ccc')"
stmt.execute "insert into foo values(4, 'ddd')"
stmt.execute "insert into foo values(5, 'eee')"

rs = stmt.execute 'SELECT * FROM foo'
rs.each do |row|
  d = []
  rs.num_result_cols.times do |i|
    d << row[i + 1]
  end
  puts d.join(',')
end

stmt.close
conn.close
env.close
