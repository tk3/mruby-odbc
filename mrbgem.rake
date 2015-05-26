MRuby::Gem::Specification.new('mruby-odbc') do |spec|
  spec.license = 'MIT'
  spec.author  = 'mruby developers'
  spec.summary = 'This is a template'
  spec.linker.libraries << 'odbc'
  # spec.add_dependency ''
end
