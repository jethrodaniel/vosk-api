# frozen_string_literal: true

MRuby::Lockfile.disable

MRuby::Build.new do |conf|
  conf.toolchain
  # conf.enable_cxx_exception

  conf.gem "."
  conf.gem File.expand_path(__dir__)
  conf.enable_test

  # needed for sample/transcribe.rb
  conf.defines << 'MRB_STR_LENGTH_MAX=0'
  conf.defines << 'MRB_ARY_LENGTH_MAX=0'
  conf.gem core: 'mruby-pack'
  conf.gem core: 'mruby-io'
  conf.gem core: 'mruby-bin-mruby'
  conf.gem core: 'mruby-bin-mirb'
end
