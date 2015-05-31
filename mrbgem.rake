MRuby::Gem::Specification.new('mruby-odbc') do |spec|
  spec.license = 'MIT'
  spec.author  = 'qtakamitsu'
  spec.summary = 'ODBC library for mruby'
  spec.linker.libraries << 'odbc'
  # spec.add_dependency ''
end
