# frozen_string_literal: true

assert("Vosk") do
  assert_equal true, Vosk.is_a?(Module)
  assert_equal true, Vosk.respond_to?(:set_log_level)

  Vosk.set_log_level(-1)
end

assert("Vosk::Model") do
  assert_equal true, Vosk.const_defined?(:Model)
  assert_equal true, Vosk::Model.is_a?(Class)
end

assert("Vosk::Recognizer") do
  assert_equal true, Vosk.const_defined?(:Recognizer)
  assert_equal true, Vosk::Recognizer.is_a?(Class)
end
