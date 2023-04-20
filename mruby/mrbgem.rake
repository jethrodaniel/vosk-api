# frozen_string_literal: true

MRuby::Gem::Specification.new("mruby-vosk") do |spec|
  spec.license = "Apache 2.0"
  spec.authors = ["Alpha Cephei Inc"]
  spec.summary = "Vosk is an offline open source speech recognition toolkit."
  spec.version = "0.3.45"

  spec.cc.include_paths << File.join(spec.dir, "../src")
  spec.linker.library_paths << "/usr/local/lib"
  spec.linker.libraries << "vosk"
end
