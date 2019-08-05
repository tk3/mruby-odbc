MRuby::Gem::Specification.new('mruby-odbc') do |spec|
	spec.version = '0.0.1'
  spec.license = 'MIT'
  spec.author  = 'qtakamitsu'
  spec.summary = 'unixODBC bindings'
  spec.linker.libraries << 'odbc'
  # spec.add_dependency ''
end
